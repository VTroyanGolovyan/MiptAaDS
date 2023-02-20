#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <memory>

enum Status {
  win,
  lose,
  proccess
};

class Cell {
  public:
    Cell() {}

    void SetContent(char content) {
      content_ = content;
    }

    char GetContent() const {
      return content_;
    }

    void Unhide() {
      hidden = false;
    }

    bool IsHidden() const {
      return hidden;
    }

    bool IsMine() const {
      return content_ == '*';
    }


  private:
    char content_{'0'};
    bool hidden{true};

};

class Map {
public:
  Map(std::vector<std::vector<std::unique_ptr<Cell>>>& map) 
    : map_(std::move(map)) {
      n_ = map_.size();
      m_ = map_.size();
  }

  const Cell& Get(int i, int j) const {
    return *map_[i][j];
  }

   Cell& Get(int i, int j) {
    return *map_[i][j];
  }

  std::vector<std::unique_ptr<Cell>>& operator[](int i) {
    return map_[i];
  }

  int Rows() const { 
    return n_;
  }

  int Columns() const { 
    return m_;
  }

  void UnhideZeroNeighbors(int i, int j) {
    std::vector<std::pair<int, int>> zeros; // фронт волны
    zeros.push_back(std::make_pair(i, j)); // фронт волны - это одна точка
    while (!zeros.empty()) {
      std::vector<std::pair<int, int>> new_zeros;
      for (auto zero : zeros) {
        int x = zero.first;
        int y = zero.second;
        for (int dx = -1; dx <= 1; ++dx) {
          for (int dy = -1; dy <= 1; ++dy) {
            if (Check(x, y, dx, dy)) {
              if (map_[x + dx][y + dy]->IsHidden()) {
                map_[x + dx][y + dy]->Unhide();
                if (map_[x + dx][y + dy]->GetContent() == '0') {
                  new_zeros.push_back(std::make_pair(x + dx, y + dy));
                }
              }
            }
          }
        }
      }
      zeros = std::move(new_zeros);
    }
  }

private:
  bool Check(int row, int column, int d_row, int d_column) const {
    if (row + d_row < 0 || row + d_row >= Rows()) {
      return false;
    }
    if (column + d_column < 0 || column + d_column >= Columns()) {
      return false;
    }
    return map_[row + d_row][column + d_column]->GetContent() != '*';
  }

  void Inc(int row, int column, int d_row, int d_column) {
    map_[row + d_row][column + d_column]->SetContent(
      map_[row + d_row][column + d_column]->GetContent() + 1
    );
  }

  std::vector<std::vector<std::unique_ptr<Cell>>> map_;
  size_t n_;
  size_t m_;
};

class MapBuilder {
  public:
    MapBuilder(int size) : size_(size) {}

    MapBuilder* GenerateCells() {
      cells_.resize(size_);
      for (int i = 0; i < size_; i++) {
        for (int j = 0; j < size_; j++) {
          cells_[i].emplace_back(std::move(CellFactory()));
        }
      }
      return this;
    }

    MapBuilder* GenerateMines(int count) {
      std::vector<std::pair<int, int>> cell_mines;
      std::random_device rd;
      std::uniform_int_distribution<int> x_gen(0, size_ - 1);
      std::uniform_int_distribution<int> y_gen(0, size_ - 1);
      for (int i = 0; i < count; ++i) {
        cell_mines.emplace_back(x_gen(rd), y_gen(rd));
      }
      for (auto& [row, column] : cell_mines) {

        if (cells_[row][column]->GetContent() == '*') {
          continue;
        }
        cells_[row][column]->SetContent('*');
        for (int dx = -1; dx <= 1; ++dx) {
          for (int dy = -1; dy <= 1; ++dy) {
            if (Check(row, column, dx, dy)) {
              Inc(row, column, dx, dy);
            }
          }
        }
      }
      return this;
    }

    Map* Build() {
      return new Map(cells_);
    }

  private:
    std::unique_ptr<Cell> CellFactory() {
      return std::make_unique<Cell>();
    }

    bool Check(int row, int column, int d_row, int d_column) const {
      if (row + d_row < 0 || row + d_row >= size_) {
        return false;
      }
      if (column + d_column < 0 || column + d_column >= size_) {
        return false;
      }
      return cells_[row + d_row][column + d_column]->GetContent() != '*';
    }

    void Inc(int row, int column, int d_row, int d_column) {
      cells_[row + d_row][column + d_column]->SetContent(
        cells_[row + d_row][column + d_column]->GetContent() + 1
      );
    }

    size_t size_;
    std::vector<std::vector<std::unique_ptr<Cell>>> cells_;
    
};

class Game {
  public:

    void Click(int i, int j) {
      if (flags_map_[i][j]) {
        return;
      }
      if (GetMap().Get(i, j).GetContent() == '0') {
        GetMap().UnhideZeroNeighbors(i, j);
      } else {
        GetMap().Get(i, j).Unhide();
        if (GetMap().Get(i, j).GetContent() == '*') {
          status_ = lose;
        }
      }
      int mine_counter = 0;
      int hide_counter = 0;
      for(int i = 0; i < GetMap().Rows(); i++){
        for(int j = 0; j < GetMap().Columns(); j++){
            if(GetMap().Get(i, j).GetContent() == '*'){
                mine_counter++;
            } 
            if(GetMap().Get(i, j).IsHidden()){
                hide_counter++;
            }
        }
      }
      if(mine_counter == hide_counter){
        status_ = win;
      }
      // check win
    }

    void ClickFlag(int i, int j) {
       flags_map_[i][j] = !flags_map_[i][j];
    }

    Map& GetMap() {
      return *map_;
    }

    std::vector<std::vector<bool>>& GetFlags() {
      return flags_map_;
    }

    int CheckStatus() {
      return status_;
    }

    static Game* GetGame() {
      if (game_ == nullptr) {
        game_ = new Game(20, 50);
      }
      return game_;
    }

    ~Game() {
      delete map_;
    }
  private:
    static Game* game_;

    Game(size_t size, size_t mines) 
      : flags_map_(size, std::vector<bool>(size, false)) {
      map_ = MapBuilder(size).GenerateCells()->GenerateMines(mines)->Build();
    }

    std::vector<std::vector<bool>> flags_map_;
    int status_{proccess};
    Map* map_;
};

Game* Game::game_{nullptr};

void DrawMap(
  sf::RenderWindow& window,  
  int size
) {
  const Map& map = Game::GetGame()->GetMap();
  std::vector<std::vector<bool>>& flags = Game::GetGame()->GetFlags();
  for (int i = 0; i < map.Rows(); ++i) {
    for (int j = 0; j < map.Columns(); ++j) {
      const Cell& cell = map.Get(i, j);
      sf::RectangleShape rectangle(sf::Vector2f(size, size));
      rectangle.setPosition(i * size + 5 * i, j * size + 5 * j);
      rectangle.setFillColor({100,100,100});
      if (!cell.IsHidden()) {
        rectangle.setFillColor({150,150,150});
      }
      window.draw(rectangle);
      if (!cell.IsHidden() && cell.GetContent() != '0') {
        sf::Text text;
        std::string text_str = "";
        text_str += cell.GetContent();
        text.setString(text_str);
        text.setPosition(i * size + 5 * i + size / 4, j * size + 5 * j + size / 4);
        text.setFillColor(sf::Color::Red);
        text.setCharacterSize(24);
        sf::Font font;
        font.loadFromFile("/Users/vhdev/MIPT_TP_SAPER/Levitation.otf");
        text.setFont(font);
        window.draw(text);
      }

      if (cell.IsHidden() && flags[i][j]) {
        sf::RectangleShape flag_rect(sf::Vector2f(size - 10, size - 10));
        flag_rect.setPosition(i * size + 5 * i + 5, j * size + 5 * j + 5);
        flag_rect.setFillColor({200,0,0});
        window.draw(flag_rect);
      }
      
    }
  }
}

void DrawWin(sf::RenderWindow& window) {
  auto size = window.getSize();
  sf::RectangleShape rectangle(sf::Vector2f(size.x, size.y));
  rectangle.setPosition(0, 0);
  rectangle.setFillColor({0,200,0});
  window.draw(rectangle);
  sf::Text text;
  std::string text_str = "You win!";
  text.setString(text_str);
  text.setPosition(100, 100);
  text.setFillColor(sf::Color::Yellow);
  text.setCharacterSize(164);
  sf::Font font;
  font.loadFromFile("/Users/vhdev/MIPT_TP_SAPER/Levitation.otf");
  text.setFont(font);
  window.draw(text);
}

void DrawLose(sf::RenderWindow& window) {
  auto size = window.getSize();
  sf::RectangleShape rectangle(sf::Vector2f(size.x, size.y));
  rectangle.setPosition(0, 0);
  rectangle.setFillColor({200,0,0});
  window.draw(rectangle);
  sf::Text text;
  std::string text_str = "You lose!";
  text.setString(text_str);
  text.setPosition(100, 100);
  text.setFillColor(sf::Color::Yellow);
  text.setCharacterSize(164);
  sf::Font font;
  font.loadFromFile("/Users/vhdev/MIPT_TP_SAPER/Levitation.otf");
  text.setFont(font);
  window.draw(text);
}
int main() {
    int cells_in_line = 20;
    
    
    sf::RenderWindow window(sf::VideoMode(cells_in_line * 55, cells_in_line * 55 + 20), "Mines!");
    
    
    while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
          if (event.type == sf::Event::Closed) {
              window.close();
          }
          if (event.type == sf::Event::MouseButtonPressed) {
              if (event.mouseButton.button == sf::Mouse::Left) {
                  int i =  event.mouseButton.x / 55;
                  int j =  event.mouseButton.y / 55;
                  Game::GetGame()->Click(i, j);   
              }
              if (event.mouseButton.button == sf::Mouse::Right) {
                  int i =  event.mouseButton.x / 55;
                  int j =  event.mouseButton.y / 55;
                  Game::GetGame()->ClickFlag(i, j);   
              }
          }
      }

      window.clear();
      if (Game::GetGame()->CheckStatus() == win) {
        DrawWin(window);
      } else if (Game::GetGame()->CheckStatus() == lose) {
        DrawLose(window);
      } else {
        DrawMap(window, 50);
      }
        


       window.display();
    }

    return 0;
}
