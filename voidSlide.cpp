//#include <stdio.h>
//#include <cs50.h>
//#include <stdbool.h>
//#include <iostream>
//#include <stdlib.h>
//#include <string>
//#include <SFML/Window.hpp>
//#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
//#include <time.h>

// i686-w64-mingw32-g++ dodgyRocket.cpp -o dodgyRocket -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
// i686-w64-mingw32-g++ -c dodgyRocket.cpp
// g++ -c dodgyRocket.cpp
// g++ voidSlide.cpp -o VoidSlide -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

int fps = 10; float velPlayer = 1200; float velPillars = -200;
float vel1Layer = velPillars*0.6; float vel2Layer = velPillars*0.3;
int sWidth = 1280; int sHeight = 800;
int numOfPillarSets = 4; float deltaT = 60.0/fps;
int pillarSeparation = 250; int secondPillarY = sHeight + pillarSeparation; 
int pillarThickness = 50; int playerWidth = 50;
float rotation = 0; float rotationSpeed = 0.2; int pillarsPassed = 0;

sf::Clock sClock;

int mode = 2; int selection = 1;
int position = sWidth + 100;
int downPressed = 0; int upPressed = 0; int rightPressed = 0; int leftPressed = 0;int enterPressed = 0; int Epressed = 0;
int sound = 1; int music = 1; int settingsSelection = 0; int highScore = 0; int lostSelection = 1;


class GameTexture {
  public:
    float posx; float posy; sf::Texture texture; sf::Texture texture2; sf::Sprite sprite;
  
  void constructTexture(GameTexture& Gtexture, int posx, int posy, float x, float y){
    sf::Sprite spriteT; Gtexture.sprite = spriteT;
    Gtexture.sprite.setTexture(Gtexture.texture);
    Gtexture.posx = posx; Gtexture.posy = posy;
    Gtexture.sprite.setTextureRect(sf::IntRect(Gtexture.posx, Gtexture.posy, x, y));
  }
  void moveSprite(GameTexture& Gtexture, float x, float y){
    Gtexture.posx = x; Gtexture.posy = y;
    Gtexture.sprite.setPosition(sf::Vector2f(Gtexture.posx, Gtexture.posy));
  }
};

class Button {
  public:
    float posx; float posy; float width; float height; int selected;
    GameTexture image; sf::RectangleShape hitbox; sf::Text text;
  void constructButton(Button& button, int posx, int posy, float width, float height){
    button.posx = posx; button.posy = posy; button.width = width; button.height = height;
    sf::RectangleShape hitboxT(sf::Vector2f(button.width, button.height));
    button.hitbox = hitboxT;
  }
  void applyTexture(Button& button){
    button.hitbox.setFillColor(sf::Color(200, 100, 200));
    button.image.texture.setRepeated(true);
    button.image.constructTexture(button.image, button.posx, button.posy, button.width, button.height);
  }
  void centerButton(Button& button){
    button.posx = sWidth/2 - button.width/2;
    button.posy = sHeight/2 - button.height/2;
    button.hitbox.setPosition(button.posx, button.posy);
    button.image.sprite.setPosition(button.posx, button.posy);
  }
  void changePosition(Button& button, float posx, float posy){
    button.posx = posx; button.posy = posy;
    button.hitbox.setPosition(button.posx, button.posy);
    button.image.sprite.setPosition(button.posx, button.posy);
  }
};

class GameSound {
  public:
  sf::SoundBuffer buffer; sf::Sound sound;
};
class Player {
  public:
    float posx = (sWidth/4.0); float posy = (sHeight/2.0);
    float momentumX = 0; float momentumY = 0; sf::RectangleShape entity;
    GameTexture image;
  void applyTexture(Player& player){
    player.image.texture.loadFromFile("resources/textures/ship.png");
    player.image.texture.setRepeated(true);
    player.image.constructTexture(player.image, player.posx-20, player.posy-25 , playerWidth + 25, playerWidth + 25);
  }
  void constructPlayer(Player& player){
    player.applyTexture(player);
    sf::RectangleShape entityT(sf::Vector2f(playerWidth, playerWidth));
    player.entity = entityT;
    player.entity.setFillColor(sf::Color(0, 200, 250));
    player.entity.setPosition(posx, posy);
    player.image.sprite.setPosition(image.posx - 5, image.posy - 5);
  }
  void changeColor(Player& player, sf::Color color){
    player.entity.setFillColor(sf::Color(color));
  }
  void playerBoundryCheck(Player& player){
    if (player.posx > sWidth - playerWidth){
    player.posx = sWidth - playerWidth; player.momentumX = 0;}
    if (player.posx < 0){
    player.posx = 0; player.momentumX = 0;}
    if (player.posy > sHeight-playerWidth){
    player.posy = sHeight-playerWidth; player.momentumY = 0;}
    if (player.posy < 0){
    player.posy = 0; player.momentumY = 0;}
  }
  void move(Player& person){ // shift/Q for missle, enter/E for bullet
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)){person.momentumY -= (velPlayer*deltaT);}
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)){person.momentumY += (velPlayer*deltaT);}
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)){person.momentumX -= (velPlayer*deltaT);}
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)){person.momentumX += (velPlayer*deltaT);}
    person.posx += person.momentumX*deltaT; person.posy += person.momentumY*deltaT;
    person.image.moveSprite(person.image, person.posx - 12.5, person.posy - 12.5);
    person.entity.setPosition(person.posx, person.posy);
    //rotation += rotationSpeed;
    //person.image.sprite.setRotation(rotation);
  }
};
class Pillars {
  public:
    float posx; float posy; float posy2; sf::RectangleShape pillar1; sf::RectangleShape pillar2;
    GameTexture image; GameTexture image2;
  void applyTexture(Pillars& pillars){
    pillars.image.texture.loadFromFile("resources/textures/pipe.png");
    pillars.image2.texture.loadFromFile("resources/textures/pipe2.png");
    pillars.image.constructTexture(pillars.image, pillars.posx, pillars.posy+600, pillarThickness + 10, 1080);
    pillars.image2.constructTexture(pillars.image2, pillars.posx, pillars.posy+600, pillarThickness + 10, 1080);
  }
  void constructRects(Pillars& pillars){
    applyTexture(pillars);
    sf::RectangleShape pillar1t(sf::Vector2f(pillarThickness, sHeight));
    sf::RectangleShape pillar2t(sf::Vector2f(pillarThickness, sHeight));
    pillars.image.sprite.setPosition(pillars.image.posx, pillars.image.posy);
    pillars.image2.sprite.setPosition(pillars.image.posx, pillars.image.posy);
    pillars.pillar1 = pillar1t;
    pillars.pillar2 = pillar2t;
    pillars.pillar1.setFillColor(sf::Color(0, 110, 250));
    pillars.pillar2.setFillColor(sf::Color(0, 250, 110));
  }
  void updateLocations(Pillars& pillar){
    pillar.posx += velPillars * deltaT;
    pillar.pillar1.setPosition(posx, posy);
    pillar.posy2 = pillar.posy + secondPillarY;
    pillar.pillar2.setPosition(posx, posy2);
    pillar.image.moveSprite(pillar.image, pillar.posx -5, pillar.posy + 1040);
    pillar.image2.moveSprite(pillar.image2, pillar.posx -5, pillar.posy2 - 1320);
  }
};

void appendNewPillar(Pillars *pillarArr, int numOfPillarSets){
  int last = numOfPillarSets - 1;
  Pillars temp = pillarArr[0];
  for (int i = 0; i < last; i++){
    pillarArr[i] = pillarArr[i+1];
  }
  pillarArr[last] = temp;
  pillarArr[last].posy = ((rand() % 6)*100) - 700;
  pillarArr[last].posx = sWidth;
}

int soundCheck(){if(sound){return 1;} return 0;}
int musicCheck(){if(music){return 1;} return 0;}


int action(){
  bool enterTapped = sf::Keyboard::isKeyPressed(sf::Keyboard::Enter);
  bool Etapped = sf::Keyboard::isKeyPressed(sf::Keyboard::E);

  if (!Etapped){Epressed = 0;}
  if (!enterTapped){enterPressed = 0;}
  if (enterTapped && (enterPressed == 0)){{enterPressed = 1; return 1;}}else if (Etapped && (Epressed == 0)){Epressed = 1; return 1;}
  return 0;
}

float deltaTime(){
  return sClock.restart().asSeconds();
}

int main(){

  int posx = sWidth/4; int posy = sHeight/2;
  // textures
  GameTexture pillar11; pillar11.texture.loadFromFile("resources/textures/pipe.png"); pillar11.texture2.loadFromFile("resources/textures/pipe2.png");
  sf::RenderWindow window(sf::VideoMode(sWidth, sHeight), "Void Slide");
  //window.setFramerateLimit(fps);

  Pillars *pillarArr = (Pillars*)malloc(sizeof(Pillars)*numOfPillarSets); // Allocating pillar array
  int startingPosx = sWidth + 100;  int separationDistance = (sWidth + 75)/numOfPillarSets;
  for (int i = 0; i < (numOfPillarSets); i++){
    Pillars j;
    pillarArr[i] = j;
    pillarArr[i].posx = startingPosx;
    pillarArr[i].posy = ((rand() % 6)*100) - 700;
    pillarArr[i].image.sprite.setTexture(pillar11.texture); pillarArr[i].image2.sprite.setTexture(pillar11.texture2);
    pillarArr[i].constructRects(pillarArr[i]);
    startingPosx += separationDistance;
  }


// menu buttons
  Button start; start.image.texture.loadFromFile("resources/textures/start.png");start.image.texture2.loadFromFile("resources/textures/start_selected.png");
  start.image.constructTexture(start.image, 0, 0, 0, 0);start.constructButton(start, 0, 0, 400, 70); start.applyTexture(start); start.centerButton(start);
  start.changePosition(start, start.posx, 400);
  
  Button settings;
  settings.image.texture.loadFromFile("resources/textures/settings.png");settings.image.texture2.loadFromFile("resources/textures/settings_selected.png");
  settings.image.constructTexture(settings.image, 0, 0, 0, 0);settings.constructButton(settings, 0, 0, 400, 70);settings.applyTexture(settings); settings.centerButton(settings);
  settings.changePosition(settings, settings.posx, start.posy+100);

  Button exit;exit.image.texture.loadFromFile("resources/textures/exit.png");exit.image.texture2.loadFromFile("resources/textures/exit_selected.png");
  exit.image.constructTexture(exit.image, 0, 0, 0, 0);exit.constructButton(exit, 0, 0, 200, 70);exit.applyTexture(exit); exit.centerButton(exit);
  exit.changePosition(exit, exit.posx, settings.posy + 100);

// sound buttons
  Button muteSounds;muteSounds.image.texture.loadFromFile("resources/textures/sound_enabled.png");muteSounds.image.texture2.loadFromFile("resources/textures/sound_disabled.png");muteSounds.image.constructTexture(muteSounds.image, 0, 0, 0, 0);
  muteSounds.constructButton(muteSounds, 0, 0, 300, 300);muteSounds.applyTexture(muteSounds); muteSounds.centerButton(muteSounds);
  muteSounds.changePosition(muteSounds, muteSounds.posx - muteSounds.width/2 - 50, 200);

  Button muteMusic;muteMusic.image.texture.loadFromFile("resources/textures/music_enabled.png");muteMusic.image.texture2.loadFromFile("resources/textures/music_disabled.png");muteMusic.image.constructTexture(muteMusic.image, 0, 0, 0, 0);
  muteMusic.constructButton(muteMusic, 0, 0, 300, 300);muteMusic.applyTexture(muteMusic); muteMusic.centerButton(muteMusic);
  muteMusic.changePosition(muteMusic, muteMusic.posx + muteMusic.width/2 + 50, 200);

// pause screen buttons

// losing screen buttons
  Button mainMenu; mainMenu.image.texture.loadFromFile("resources/textures/mainMenu.png");mainMenu.image.texture2.loadFromFile("resources/textures/start_selected.png");
  mainMenu.image.constructTexture(mainMenu.image, 0, 0, 0, 0);mainMenu.constructButton(mainMenu, 0, 0, 300, 150); mainMenu.applyTexture(mainMenu); mainMenu.centerButton(mainMenu);
  mainMenu.changePosition(mainMenu, (mainMenu.posx) - 50 - (mainMenu.width/2), 500);

  Button exitLost; exitLost.image.texture.loadFromFile("resources/textures/lostLeave.png");exitLost.image.texture2.loadFromFile("resources/textures/start_selected.png");
  exitLost.image.constructTexture(exitLost.image, 0, 0, 0, 0);exitLost.constructButton(exitLost, 0, 0, 300, 150); exitLost.applyTexture(exitLost); exitLost.centerButton(exitLost);
  exitLost.changePosition(exitLost, exitLost.posx + 50 + (exitLost.width/2), 500);

  Player player; player.constructPlayer(player); // player with construction  
  
  GameTexture title; title.texture.loadFromFile("resources/textures/title.png"); title.constructTexture(title, 0, 0, 600, 200); title.sprite.setPosition(340, 120);
  GameTexture soundSelector; soundSelector.texture.loadFromFile("resources/textures/sound_selected.png"); soundSelector.constructTexture(soundSelector, 0, 0, muteSounds.width, muteSounds.height);
  GameTexture lostSelector; lostSelector.texture.loadFromFile("resources/textures/lostSelect.png"); lostSelector.constructTexture(lostSelector, 0, 0, mainMenu.width, mainMenu.height);
  GameTexture controls; controls.texture.loadFromFile("resources/textures/controls.png"); controls.constructTexture(controls, 0, 0, 219, 253);
  GameTexture background; background.texture.loadFromFile("resources/textures/background.png"); background.constructTexture(background, 0, 0, 2048, sHeight*2); background.texture.setRepeated(true);

  GameSound gameMusic; gameMusic.buffer.loadFromFile("resources/sounds/gameMusic.wav"); gameMusic.sound.setBuffer(gameMusic.buffer); gameMusic.sound.setLoop(true); gameMusic.sound.setVolume(20);
  GameSound crash; crash.buffer.loadFromFile("resources/sounds/crash.wav"); crash.sound.setBuffer(crash.buffer);
  GameSound point; point.buffer.loadFromFile("resources/sounds/point.wav"); point.sound.setBuffer(point.buffer);
  GameSound powerup; powerup.buffer.loadFromFile("resources/sounds/powerup.wav"); powerup.sound.setBuffer(powerup.buffer);
  GameSound selectionSound; selectionSound.buffer.loadFromFile("resources/sounds/selection.wav"); selectionSound.sound.setBuffer(selectionSound.buffer);
  GameSound titleMusic; titleMusic.buffer.loadFromFile("resources/sounds/titleMusic.wav"); titleMusic.sound.setBuffer(titleMusic.buffer); titleMusic.sound.setLoop(true); titleMusic.sound.setVolume(20);

  sf::Event event;
  controls.sprite.setPosition(1000, 200);

  sf::Font font; font.loadFromFile("resources/font.ttf");
  sf::Text highScoreText; highScoreText.setFont(font);
  sf::Text scoreText; scoreText.setFont(font);
  
  sf::SoundSource::Status soundIsPlaying= sf::SoundSource::Status::Playing;
  sf::SoundSource::Status soundIsPaused = sf::SoundSource::Status::Paused;
  sf::SoundSource::Status soundIsStopped = sf::SoundSource::Status::Stopped;

  // 1 = menu | 2 = game
  while (window.isOpen()){
    // printf("%f\n", 1/deltaT);
    window.pollEvent(event);
    deltaT = deltaTime();
    if (!music){
      if (gameMusic.sound.getStatus() == soundIsPlaying){gameMusic.sound.stop();}
      if (titleMusic.sound.getStatus() == soundIsPlaying){titleMusic.sound.stop();}
    }
    if ((event.type == sf::Event::Closed)){
      free(pillarArr);window.close();break;}
    if (mode == 1){
      if ((gameMusic.sound.getStatus() == soundIsPaused || gameMusic.sound.getStatus() == soundIsStopped) && (music)){gameMusic.sound.play();}
      if (action()){
        if(sound){selectionSound.sound.play();}
        gameMusic.sound.pause();
      }
      int lost = 0;
      //player movement
      player.move(player);
      player.playerBoundryCheck(player);
      

      for (int i = 0; i < (numOfPillarSets); i++){pillarArr[i].updateLocations(pillarArr[i]);} // pillar movement
      if (pillarArr[0].posx <= -75){appendNewPillar(pillarArr, numOfPillarSets);pillarsPassed += 1; if(sound){point.sound.play();}} // Pillar circulation loop
      // Next frame
      window.clear(); // Clear window
      if (background.posx <= -511.9){
        background.posx = 0;
      }
      background.moveSprite(background, background.posx + vel2Layer*deltaT, 0);
      window.draw(background.sprite);
      window.draw(player.entity); window.draw(player.image.sprite); // draw player
      window.draw(scoreText);
      for (int i = 0; i < (numOfPillarSets); i++){ // pillar drawing loop
        Pillars currSet = pillarArr[i];
        window.draw(currSet.pillar1);
        window.draw(currSet.pillar2);
      //  printf("%f, %f, %f\n", currSet.image.posy, currSet.image2.posy, currSet.posy2);
        currSet.image.constructTexture(currSet.image, 0, 0, pillarThickness+10,1080);
        currSet.image.moveSprite(currSet.image, currSet.posx-5, currSet.posy2 - 5);
        currSet.image2.constructTexture(currSet.image2, 0, 0, pillarThickness+10,1080);
        currSet.image2.moveSprite(currSet.image2, currSet.posx-5, currSet.posy - (1075-sHeight));
        window.draw(currSet.image.sprite);
        window.draw(currSet.image2.sprite);
      }
      // pillar collision loop
      int playerX = player.posx;
      int playerY = player.posy;
      for (int i = 0; i < (numOfPillarSets); i++){
        int pillarX = pillarArr[i].posx; int pillar2Y = pillarArr[i].posy2;
        if ((playerX >= pillarX - playerWidth) && (playerX < pillarX + pillarThickness)){
        if ((playerY >= pillar2Y - playerWidth) || (playerY <= pillar2Y - pillarSeparation)){
          player.changeColor(player, sf::Color(200, 10, 10)); lost = 1;}
        else{player.changeColor(player, sf::Color(0, 200, 250));}}else{player.changeColor(player, sf::Color(0, 200, 250));}}
      //printf("%i\n", pillarsPassed);
      if (lost){
        player.posx = sWidth/4; player.posy = sHeight/2; player.momentumX = 0; player.momentumY = 0; 
        float temp = pillarArr[0].posx;
        if (highScore < pillarsPassed){highScore = pillarsPassed;}
        //scoreText.setString("Your Score: %i", pillarsPassed);highScoreText.setString("High Score: %i", highScore);
        for (int i = 0; i < numOfPillarSets; i++){
          pillarArr[i].posx += sWidth-temp;
        }
        gameMusic.sound.stop();
        if(sound){crash.sound.play();}
        pillarsPassed = 0; mode = 4;}
      // End the current frame and display its contents on screen
      window.display();
    }
    else{
      window.clear();
      window.draw(background.sprite);
      background.moveSprite(background, 0, background.posy + (vel2Layer*deltaT)/4);
      if (background.posy >= -512.1 && background.posy <= -511.9){background.posy = 0;}
    if (mode == 2){ // main menu
      if ((titleMusic.sound.getStatus() == soundIsPaused || titleMusic.sound.getStatus() == soundIsStopped) && (music)){titleMusic.sound.play();}
      if (titleMusic.sound.getStatus() == sf::SoundSource::Status::Stopped && (music)){titleMusic.sound.play();}
      bool upTapped = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);bool downTapped = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
      if (upTapped && (upPressed == 0)){if (selection > 1){selection -= 1; upPressed = 1;}else{selection = 3; upPressed = 1;}}
      else if (downTapped && (downPressed== 0)){if (selection < 3){selection += 1; downPressed = 1;}else{selection = 1; downPressed = 1;}}
      if (!(upTapped)){upTapped = 0;upPressed = 0;} if (!(downTapped)){downTapped = 0;downPressed = 0;}
      //selection highlighting
      if (selection == 1){start.image.sprite.setTexture(start.image.texture2);settings.image.sprite.setTexture(settings.image.texture);exit.image.sprite.setTexture(exit.image.texture);start.hitbox.setFillColor(sf::Color(100, 100, 255));settings.hitbox.setFillColor(sf::Color(200, 100, 200));exit.hitbox.setFillColor(sf::Color(200, 100, 200));if (action()){mode = 1; if(sound){selectionSound.sound.play();} titleMusic.sound.stop();}}
      else if (selection == 2){start.image.sprite.setTexture(start.image.texture);settings.image.sprite.setTexture(settings.image.texture2);exit.image.sprite.setTexture(exit.image.texture);start.hitbox.setFillColor(sf::Color(200, 100, 200));settings.hitbox.setFillColor(sf::Color(100, 100, 255));exit.hitbox.setFillColor(sf::Color(200, 100, 200));if (action()){if(sound){selectionSound.sound.play();}mode = 3;}}
      else if (selection == 3){start.image.sprite.setTexture(start.image.texture);settings.image.sprite.setTexture(settings.image.texture);exit.image.sprite.setTexture(exit.image.texture2);start.hitbox.setFillColor(sf::Color(200, 100, 200));settings.hitbox.setFillColor(sf::Color(200, 100, 200));exit.hitbox.setFillColor(sf::Color(100, 100, 255));if (action()){titleMusic.sound.stop();if(sound){selectionSound.sound.play();}free(pillarArr);window.close();break;}}
      //printf("%i\n", selection);
      window.draw(start.hitbox); window.draw(start.image.sprite);
      window.draw(settings.hitbox); window.draw(settings.image.sprite);
      window.draw(exit.hitbox); window.draw(exit.image.sprite);
      window.draw(title.sprite);
      window.draw(controls.sprite);
      window.display();
    }
    else if (mode == 3){
      if ((titleMusic.sound.getStatus() == soundIsPaused || titleMusic.sound.getStatus() == soundIsStopped) && (music)){titleMusic.sound.play();}
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)){mode = 2; settingsSelection = 0;}
      window.clear();
      bool rightTapped = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);bool leftTapped = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
      if (rightTapped && (rightPressed == 0)){if (settingsSelection == 0){settingsSelection = 1; rightPressed = 1;}else{settingsSelection  = 0; rightPressed = 1;}}
      else if (leftTapped && (leftPressed == 0)){if (settingsSelection == 0){settingsSelection = 1; leftPressed = 1;}else{settingsSelection = 0; leftPressed = 1;}}
      if (!(rightTapped)){rightTapped = 0;rightPressed = 0;} if (!(leftTapped)){leftTapped = 0;leftPressed = 0;}
      if (settingsSelection){
        soundSelector.sprite.setPosition(muteMusic.posx, muteMusic.posy);
        if (action()){if(sound){selectionSound.sound.play();} music = !music;}
        if (!music){muteMusic.image.sprite.setTexture(muteMusic.image.texture2);}else{muteMusic.image.sprite.setTexture(muteMusic.image.texture);}}
      else {
        soundSelector.sprite.setPosition(muteSounds.posx, muteSounds.posy);
        if (action()){if(sound){selectionSound.sound.play();} sound = !sound;}
        if (!sound){muteSounds.image.sprite.setTexture(muteSounds.image.texture2);}else{muteSounds.image.sprite.setTexture(muteSounds.image.texture);}}
      window.draw(background.sprite);
      window.draw(muteSounds.hitbox); window.draw(muteSounds.image.sprite);
      window.draw(muteMusic.hitbox); window.draw(muteMusic.image.sprite);
      window.draw(soundSelector.sprite);
      window.draw(controls.sprite);
      window.display();
    }
    else if (mode == 4){// losing screen
      bool rightTapped = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);bool leftTapped = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
      if (rightTapped && (rightPressed == 0)){if (lostSelection == 0){lostSelection = 1; rightPressed = 1;}else{lostSelection  = 0; rightPressed = 1;}}
      else if (leftTapped && (leftPressed == 0)){if (lostSelection == 0){lostSelection = 1; leftPressed = 1;}else{lostSelection = 0; leftPressed = 1;}}
      if (!(rightTapped)){rightTapped = 0;rightPressed = 0;} if (!(leftTapped)){leftTapped = 0;leftPressed = 0;}
      
      if (lostSelection){
        lostSelector.sprite.setPosition(mainMenu.posx, mainMenu.posy);
        if (action()){mode = 2; if(sound){selectionSound.sound.play();}}}
      else {
        lostSelector.sprite.setPosition(exitLost.posx, exitLost.posy);
        if (action()){if(sound){selectionSound.sound.play();}free(pillarArr);window.close();break;}}
      
      window.draw(mainMenu.image.sprite); window.draw(exitLost.image.sprite); window.draw(lostSelector.sprite);
      window.display();
    }
  };}
};

//test comment
