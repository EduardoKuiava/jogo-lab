#include <SFML/Graphics.hpp>

enum carStatus{
    Driving,
    Turningright, //struct para definir estado do carro, não foi necessario
    Turningleft,
    Crash
};

int main()
{
    const int windowWidgth = 800, windowHeight = 600; //tamanho janela

    sf::RenderWindow window(sf::VideoMode(windowWidgth, windowHeight), "Meu Game");
    window.setFramerateLimit(60);//limita frames

    sf::Clock clock;

    sf::Texture bgTexture;// background
    bgTexture.loadFromFile("assets/800x600.png");
    bgTexture.setRepeated(true);
    sf::IntRect bgRect(0, 0, windowWidgth, windowWidgth);
    sf::Sprite bgSprite(bgTexture);

    sf::Texture bordasTexture;// uma parte do background sem a estrada pra podes fazer barreiras prro carro não sair
    bordasTexture.loadFromFile("assets/bordadir.png");
    bordasTexture.setRepeated(true);
    sf::IntRect bordasRect(0, 0, 235, 750);
    sf::Sprite bordasSprite(bordasTexture);

    sf::Texture bordas2Texture;// uma parte do background sem a estrada pra podes fazer barreiras prro carro não sair
    bordas2Texture.loadFromFile("assets/bordaesq.png");
    bordas2Texture.setRepeated(true);
    sf::IntRect bordas2Rect(0, 0, 313, 750);
    sf::Sprite bordas2Sprite(bordas2Texture);

    sf::Texture carTexture;// carro
    carTexture.loadFromFile("assets/spritesm.png");
    sf::IntRect carRect(0, 2, 30, 34);
    sf::Sprite carSprite(carTexture);

    sf::Texture oilTexture;// poças de oleo que diminui pontuaçao
    oilTexture.loadFromFile("assets/spritesm.png");
    sf::IntRect oilRect(212, 146, 38, 34);
    sf::Sprite oilSprite(oilTexture);
    sf::Sprite oil1Sprite(oilTexture);
    sf::Sprite oil2Sprite(oilTexture);
    sf::Sprite oil3Sprite(oilTexture);

    sf::Texture oilkillTexture;//poças de oleo que dao game over
    oilkillTexture.loadFromFile("assets/spritesm.png");
    sf::IntRect oilkillRect(219, 185, 28, 26);
    sf::Sprite oilkillSprite(oilTexture);
    sf::Sprite oilkill2Sprite(oilTexture);

    sf::Texture chegadaTexture;// linha de chegada
    chegadaTexture.loadFromFile("assets/cheee.png");
    sf::Sprite chegadaSprite(chegadaTexture);

    sf::Font font;
    font.loadFromFile("assets/game_over.ttf");
    sf::Text text;
    text.setFont(font);// "game over" e "vitoria"
    text.setCharacterSize(200);
    text.setFillColor(sf::Color::White);
    text.setPosition(200,100);
    sf::Text text2;
    text2.setFont(font);//contador 
    text2.setCharacterSize(200);
    text2.setFillColor(sf::Color::White);
    text2.setPosition(642,windowHeight - 700);
    sf::Text text3;
    text3.setFont(font);// texto de "pts"
    text3.setCharacterSize(150);
    text3.setFillColor(sf::Color::White);
    text3.setPosition(700,windowHeight - 594);
    text3.setString("pts");
    sf::Text text4;
    text4.setFont(font);// texto de "pts"
    text4.setCharacterSize(150);
    text4.setFillColor(sf::Color::White);
    text4.setPosition(700,windowHeight - 594);
    text4.setString("pts");

    
    float oilY = windowHeight - 630;//coordenada de inicio da poça de oleo
    float oilY1 = windowHeight - 630;//coordenada de inicio da poça de oleo
    float oilY2 = windowHeight - 1400;//coordenada de inicio da poça de oleo
    float oilY3 = windowHeight - 650;//coordenada de inicio da poça de oleo
    float oilYk = windowHeight - 779;//coordenada de inicio da poça de oleo
    float oilYk2 = windowHeight - 2020;//coordenada de inicio da poça de oleo
    float carX = 420;//inicio do carro
    float che = windowHeight - 10000;//linha de chegada
    bool gameOver = false;//quando morre a bool vira true fazendo com que não entre no loop(usei pra vitoria tambem)

    int steps = 0;//contador dos pontos

    carStatus carCurrentStatus = carStatus::Driving;

    // game loop
    while (window.isOpen())//loop
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)//fechar a janela pelo x
                window.close();
            else if (event.type == sf::Event::KeyPressed) {//fechar a janela com o esc
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();     
            }            
        }

        //Atualização do estado do Jogo
        
        if(!gameOver){//se game over false entao entra no loop
        bgRect.top--; //usei isso pra fazer com que o fundo se mova
        bgRect.top--;//repeti porque deira mais rapido
        bgRect.top--;
        bgRect.top--;
        bgRect.top--; 

        bordasRect.top--;//movimenta borda direita
        bordasRect.top--;
        bordasRect.top--;
        bordasRect.top--;
        bordasRect.top--;
        

        bordas2Rect.top--;//movimenta borda esquerda
        bordas2Rect.top--;
        bordas2Rect.top--;
        bordas2Rect.top--;
        bordas2Rect.top--;

        bgSprite.setTextureRect(bgRect);
        bordasSprite.setTextureRect(bordasRect);
        bordas2Sprite.setTextureRect(bordas2Rect);
        bordasSprite.setPosition(565, 0);//posiçao borda direita
        bordas2Sprite.setPosition(0, 0);//posiçao boorda esquerda

        if (clock.getElapsedTime().asSeconds() >= 0.01) {        
            steps++; //acumula pontos
            text2.setString(std::to_string(steps/8));
            clock.restart();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){//vira pra direita
            carCurrentStatus = carStatus::Turningright;
            carX = carX + 10;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){//vira pra esquerda
            carCurrentStatus = carStatus::Turningleft;
            carX = carX - 10;
        }

        if(oilY > 605) oilY = -600;//quanda as poças saem da tela essa parte faz com que elas tenham uma nova posiçao pra voltar
        if(oilY1 > 605) oilY1 = -600;
        if(oilY2 > 605) oilY2 = -1000;
        if(oilY3 > 605) oilY3 = -300;
        if(oilYk > 605) oilYk = -750;
        if(oilYk2 > 605) oilYk2 = -2000;

        oilY++, oilY1++, oilY2++, oilY3++, oilYk++, oilYk2++, che++;//movimento das poças e chegada
        oilY++, oilY1++, oilY2++, oilY3++, oilYk++, oilYk2++, che++;
        oilY++, oilY1++, oilY2++, oilY3++, oilYk++, oilYk2++, che++;
        oilY++, oilY1++, oilY2++, oilY3++, oilYk++, oilYk2++, che++;
        oilY++, oilY1++, oilY2++, oilY3++, oilYk++, oilYk2++, che++;

        oilSprite.setTextureRect(oilRect);
        oil1Sprite.setTextureRect(oilRect);
        oil2Sprite.setTextureRect(oilRect);
        oil3Sprite.setTextureRect(oilRect);
        oilkillSprite.setTextureRect(oilkillRect);
        oilkill2Sprite.setTextureRect(oilkillRect);

        oilSprite.setPosition(345, oilY);//recebe valores que fazem os spriters
        oil1Sprite.setPosition(510, oilY1);
        oil2Sprite.setPosition(450, oilY2);
        oil3Sprite.setPosition(380, oilY3);
        oilkillSprite.setPosition(400, oilYk);
        oilkill2Sprite.setPosition(420, oilYk2);
        chegadaSprite.setPosition(170, che);

        carSprite.setTextureRect(carRect);
        carSprite.setPosition(carX, windowHeight -94);//posiciona o carro + ou - no centro

        if(carSprite.getGlobalBounds().intersects(oilkillSprite.getGlobalBounds())){//se toca na possa da game over
            gameOver = true;
            text.setString("GAME OVER !");
        }
        else if(carSprite.getGlobalBounds().intersects(oilkill2Sprite.getGlobalBounds())){//se toca na possa da game over
            gameOver =true;
            text.setString("GAME OVER !");
        }
        else if(carSprite.getGlobalBounds().intersects(oilSprite.getGlobalBounds())){//se toca nessa poça diminui os pontos
            

            //text.setString("- 10 pts");
            steps = steps - 10; 
        }
         else if(carSprite.getGlobalBounds().intersects(oil1Sprite.getGlobalBounds())){//se toca nessa poça diminui os pontos
            
            
            //text.setString("- 10 pts");
            steps = steps - 10; 
        }
         else if(carSprite.getGlobalBounds().intersects(oil2Sprite.getGlobalBounds())){//se toca nessa poça diminui os pontos
            
           
            //text.setString("- 10 pts");
            steps = steps - 10; 
        }
         else if(carSprite.getGlobalBounds().intersects(oil3Sprite.getGlobalBounds())){//se toca nessa poça diminui os pontos
            
            
            //text.setString("- 10 pts");
            steps = steps - 10; 
        }
         else if(carSprite.getGlobalBounds().intersects(chegadaSprite.getGlobalBounds())){
            gameOver = true;
            text.setString("VITORIA !");// quando o carro encosta na linha de chegada tudo para e da mensagem de vitoria
            
        }
         if(carSprite.getGlobalBounds().intersects(bordasSprite.getGlobalBounds())){//se toca na borda da game over
            gameOver = true;
            text.setString("GAME OVER !");
        }
         if(carSprite.getGlobalBounds().intersects(bordas2Sprite.getGlobalBounds())){//se toca na borda da game over
            gameOver = true;
            text.setString("GAME OVER !");
        }
        
        

        
        
        }
        // Desenha o Frame
        window.clear();
        window.draw(bgSprite);//faz tudo apareceer
        window.draw(bordasSprite);
        window.draw(bordas2Sprite);
        window.draw(oilSprite);
        window.draw(oil1Sprite);
        window.draw(oil2Sprite);
        window.draw(oil3Sprite);
        window.draw(oilkillSprite);
        window.draw(oilkill2Sprite);
        window.draw(text);
        window.draw(text2);
        window.draw(text3);
        window.draw(text4);
        window.draw(chegadaSprite);
        window.draw(carSprite);  
        window.display();
    }

    return 0;
}