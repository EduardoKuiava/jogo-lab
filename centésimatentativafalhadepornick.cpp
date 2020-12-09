#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <vector>
using namespace std;

enum carStatus{
    Driving,
    Turningright, //struct para definir estado do carro
    Turningleft,
    Crash
};

struct dados
{
    string nomes;
    int pontos;
};
dados ranking;

bool cmp(const dados &a, const dados &b) {
    return a.pontos > b.pontos || (a.pontos == b.pontos && a.nomes < b.nomes);
}

string salvarNomes;
int salvarPontos; 


int main()
{
    const int windowWidth = 800, windowHeight = 600; //tamanho janela

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Meu Game");
    window.setFramerateLimit(60);//limita frames

    sf::Clock clock;

    sf::Texture bgTexture;// background
    bgTexture.loadFromFile("assets/800x600.png");
    bgTexture.setRepeated(true);
    sf::IntRect bgRect(0, 0, windowWidth, windowHeight);
    sf::Sprite bgSprite(bgTexture);

    sf::Texture bordasTexture;//parte direita do background sem a estrada pra fazer barreiras prro carro não sair
    bordasTexture.loadFromFile("assets/bordadir.png");
    bordasTexture.setRepeated(true);
    sf::IntRect bordasRect(0, 0, 235, 750);
    sf::Sprite bordasSprite(bordasTexture);

    sf::Texture bordas2Texture;//parte esquerda do background sem a estrada pra fazer barreiras prro carro não sair
    bordas2Texture.loadFromFile("assets/bordaesq.png");
    bordas2Texture.setRepeated(true);
    sf::IntRect bordas2Rect(0, 0, 313, 750);
    sf::Sprite bordas2Sprite(bordas2Texture);

    sf::Texture nameScreenTexture; // Textura do Nickname
    nameScreenTexture.loadFromFile("assets/nameScreen.png");
    sf::Sprite nameSprite(nameScreenTexture);

    sf::Texture carTexture;// carro
    carTexture.loadFromFile("assets/spritesm.png");
    bgTexture.setRepeated(true);
    sf::IntRect carRect(0, 2, 30, 34);
    sf::Sprite carSprite(carTexture);

    sf::Texture oilTexture;// poças de oleo que diminui pontuaçao
    oilTexture.loadFromFile("assets/spritesm.png");
    sf::IntRect oilRect(212, 146, 38, 34);
    sf::Sprite oilSprite(oilTexture);
    sf::Sprite oil1Sprite(oilTexture);
    sf::Sprite oil2Sprite(oilTexture);
    sf::Sprite oil3Sprite(oilTexture);

    sf::Texture oilkillTexture;//poças de oleo que dão game over
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
    text.setOutlineColor(sf::Color::Black);
    text.setFillColor(sf::Color::White);
    text.setPosition(200,100);
    sf::Text text2;
    text2.setFont(font);//contador 
    text2.setCharacterSize(200);
    text2.setOutlineColor(sf::Color::Black);
    text2.setFillColor(sf::Color::White);
    text2.setPosition(642,windowHeight - 700);
    sf::Text text3;
    text3.setFont(font);// texto de "pts"
    text3.setCharacterSize(150);
    text3.setOutlineColor(sf::Color::Black);
    text3.setFillColor(sf::Color::White);
    text3.setPosition(700,windowHeight - 594);
    text3.setString("pts");
    

    sf::String playerInput;
    sf::Text playerText;
    playerText.setFont(font);
    playerText.setCharacterSize(42);
    playerText.setOutlineColor(sf::Color::Black);
    playerText.setFillColor(sf::Color::White);

    // Criação do vetor para o ranking
    vector<dados>rankingFinal;

        //Partidas anteriores
        ifstream entrada;
        entrada.open("ranking.txt");

        if (entrada) {
            while (entrada >> ranking.nomes) {
                entrada >> ranking.pontos;
                rankingFinal.push_back(ranking);
            }
            entrada.close();  
        }   
        rankingFinal.push_back(ranking);
        salvarNomes = ranking.nomes;
        salvarPontos = ranking.pontos;
    

    string nome;// variavel do tipo score que foi criado acima
    float oilY = windowHeight - 630;//coordenada de inicio da poça de oleo
    float oilY1 = windowHeight - 630;//coordenada de inicio da poça de oleo
    float oilY2 = windowHeight - 1400;//coordenada de inicio da poça de oleo
    float oilY3 = windowHeight - 650;//coordenada de inicio da poça de oleo
    float oilYk = windowHeight - 779;//coordenada de inicio da poça de oleo
    float oilYk2 = windowHeight - 2020;//coordenada de inicio da poça de oleo
    float carX = 420;//inicio do carro
    float che = windowHeight - 10000;//linha de chegada
    bool aberto = false;
    bool nickdigitado = true;
    bool gameOver = true;//quando morre a bool vira true fazendo com que não entre no loop(usei pra vitoria tambem)
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
                else if (event.type == sf::Event::TextEntered) {
                    if (playerInput.getSize() < 10) {
                        playerInput.replace(sf::String(" "), sf::String(""));
                        playerInput.replace(sf::String(""), sf::String(""));
                        playerInput +=event.text.unicode;
                        playerText.setString(playerInput);
                        nome = playerText.getString();
                    }
                }
                else if (event.key.code == sf::Keyboard::F2){
                    gameOver = false;
                    bgRect = {0, 0, windowWidth, windowHeight};
                    bordasRect = {0, 0, 235, 750};
                    bordas2Rect = {0, 0, 313, 750};
                    text.setString("");
                    steps = 0;
                    oilY = windowHeight - 630;
                    oilY1 = windowHeight - 630;
                    oilY2 = windowHeight - 1400;
                    oilY3 = windowHeight - 650;
                    oilYk = windowHeight - 779;
                    oilYk2 = windowHeight - 2020;
                    carX = 420;
                    che = windowHeight - 10000;
                    carSprite.setPosition(420, windowHeight -94);
                    oilSprite.setPosition(345, oilY);
                    oil1Sprite.setPosition(510, oilY1);
                    oil2Sprite.setPosition(450, oilY2);
                    oil3Sprite.setPosition(380, oilY3);
                    oilkillSprite.setPosition(400, oilYk);
                    oilkill2Sprite.setPosition(420, oilYk2);
                    chegadaSprite.setPosition(170, che);   
                }
            }            
        }

        // Enquanto estiver na tela de Nickname
        if(nickdigitado && gameOver) {
            playerText.setPosition(240, 420);
            if (event.key.code == sf::Keyboard::F1) {
                //nickdigitado = false;
                gameOver = false;
            }
        }


        //Atualização do estado do Jogo
        
        if(!gameOver && !nickdigitado){//se game over false entao entra no loop

        if (!aberto) {
            aberto = true;
            ifstream entrada;
            entrada.open("ranking.txt");

            if (entrada) {
                while (entrada >> ranking.nomes) {
                    entrada >> ranking.pontos;
                    entrada.ignore();
                    if (ranking.pontos > 0) {
                    rankingFinal.push_back(ranking);
                    }
                }
                entrada.close();
                
                // Salvar nick e score nas variaveis do ranking
                salvarNomes = ranking.nomes;
                salvarPontos = ranking.pontos;
                ofstream saida;
                saida.open("ranking.txt");
                    for (auto i : rankingFinal){
                        saida << i.nomes << " " << i.pontos << endl;
                    }
                    saida.close();
                }
            }

            ranking.nomes = nome;
            playerInput.clear();

        bgRect.top--; //usei isso pra fazer com que o fundo se mova
        bgRect.top--;//repeti porque deixa mais rapido
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
            text2.setString(std::to_string(steps/6));
            clock.restart();
        }
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){//vira pra direita
            carX = carX + 10;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){//vira pra esquerda
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

        oilSprite.setPosition(345, oilY);// posição das poças
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
            
            steps = steps - 10; 
        }
         else if(carSprite.getGlobalBounds().intersects(oil1Sprite.getGlobalBounds())){//se toca nessa poça diminui os pontos
            
            steps = steps - 10; 
        }
         else if(carSprite.getGlobalBounds().intersects(oil2Sprite.getGlobalBounds())){//se toca nessa poça diminui os pontos
          
            steps = steps - 10; 
        }
         else if(carSprite.getGlobalBounds().intersects(oil3Sprite.getGlobalBounds())){//se toca nessa poça diminui os pontos
            
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
        // Salvar score no ranking
        if (salvarPontos < steps) {
            ranking.pontos = steps;
            rankingFinal.pop_back();
            rankingFinal.push_back(ranking);
            sort(rankingFinal.begin(), rankingFinal.end(), cmp);
            ofstream saida;
            saida.open("ranking.txt"); 
                for (auto i : rankingFinal){
                    saida << i.nomes << " " << i.pontos << endl;
                }
                saida.close();

                ifstream entrada;
                entrada.open("ranking.txt");

                if (entrada) {
                    rankingFinal.clear();
                    while (entrada >> ranking.nomes) {
                    entrada >> ranking.pontos;
                    entrada.ignore();
                        if (ranking.pontos > 0) {
                            rankingFinal.push_back(ranking);
                            }
                    }
                    entrada.close();

                    ofstream saidaf;
                    saidaf.open("ranking.txt");
                        for (auto i : rankingFinal){
                            saidaf << i.nomes << " " << i.pontos  << endl;
                        }
                    saidaf.close();
                }
            
        }
        
        

        
        
        }
        // Desenha o Frame
        window.clear();
        if(nickdigitado) {
            window.draw(nameSprite);
            // quando escrever o nickname atualizar para true
            window.draw(playerText);
        }
        
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
        window.draw(chegadaSprite);
        window.draw(carSprite);  
        window.display();
        
    }

    return 0;
}