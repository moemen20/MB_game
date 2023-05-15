#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include<SFML/Audio.hpp>
//#include <windows.h>
constexpr int N = 8, M = 8,
TILESIZE = 64,
WIDTH = N * TILESIZE,
HEIGHT = M * TILESIZE;
bool g_win = false;
int MAP[N][M];
int score = 0;

sf::Vector2i genPos()
{
    sf::Vector2i v;
    while (1)
    {
        v.x = rand() % N;
        v.y = rand() % N;
        if (MAP[v.y][v.x] == 0)
            break;
    }
    return v;
}

enum class Direction
{
    Left,
    Right,
    Up,
    Down,
};

void coutMap()
{
    for (int a = 0; a < N; a++)
    {
        for (int b = 0; b < M; b++)
            std::cout << MAP[a][b] << " ";
        std::cout << "\n";
    }
    std::cout << "\n";
}

int genNewTile()
{
    int t = rand() % 2;
    if (t == 0)
        return 2;
    else
        return 4;
}

void placeNewTile()
{
    sf::Vector2i p = genPos();
    MAP[p.y][p.x] = genNewTile();
}

void shift(Direction d)
{
    bool didShift = 0;
    if (d == Direction::Up)
    {
        for (int j = 0; j < M; j++) // Every row
        {
            for (int i = 0; i < N - 1; i++) // Search for every 0 (empty tile) and remove it
                if (MAP[i][j] == 0)
                {
                    bool change = 0;
                    for (int k = i; k < N - 1; k++) // Shift everything up
                    {
                        MAP[k][j] = MAP[k + 1][j];
                        if (MAP[k][j] != 0)
                        {
                            didShift = 1;
                            change = 1;
                        }
                    }
                    MAP[N - 1][j] = 0;
                    if (change) i--; // If something really changed (did not shift only 0s) check again the current position
                }
            for (int i = 0; i < N - 1; i++) // Merge the tiles with the same number
                if (MAP[i][j] == MAP[i + 1][j] && MAP[i][j] != 0)
                {
                    didShift = 1;
                    for (int k = i; k < N - 1; k++)
                        MAP[k][j] = MAP[k + 1][j];
                    MAP[N - 1][j] = 0;
                    MAP[i][j] *= 2;
                    score += MAP[i][j];
                }
        }
    }
    else if (d == Direction::Left)
    {
        for (int i = 0; i < N; i++) // Every line
        {
            for (int j = 0; j < M - 1; j++) // Search for every 0 (empty tile) and remove it
                if (MAP[i][j] == 0)
                {
                    bool change = 0;
                    for (int k = j; k < M - 1; k++) // Shift everything up
                    {
                        MAP[i][k] = MAP[i][k + 1];
                        if (MAP[i][k] != 0)
                        {
                            didShift = 1;
                            change = 1;
                        }
                    }
                    MAP[i][M - 1] = 0;
                    if (change) j--; // If something really changed (did not shift only 0s) check again the current position
                }
            for (int j = 0; j < N - 1; j++) // Merge the tiles with the same number
                if (MAP[i][j] == MAP[i][j + 1] && MAP[i][j] != 0)
                {
                    didShift = 1;
                    for (int k = j; k < M - 1; k++)
                        MAP[i][k] = MAP[i][k + 1];
                    MAP[i][M - 1] = 0;
                    MAP[i][j] *= 2;
                    score += MAP[i][j];
                }
        }
    }
    else if (d == Direction::Down)
    {
        for (int j = 0; j < M; j++) // Every row
        {
            for (int i = N - 1; i >= 0; i--) // Search for every 0 (empty tile) and remove it
                if (MAP[i][j] == 0)
                {
                    bool change = 0;
                    for (int k = i; k > 0; k--) // Shift everything down
                    {
                        MAP[k][j] = MAP[k - 1][j];
                        if (MAP[k][j] != 0)
                        {
                            didShift = 1;
                            change = 1;
                        }
                    }
                    MAP[0][j] = 0;
                    if (change) i++; // If something really changed (did not shift only 0s) check again the current position
                }
            for (int i = N - 1; i > 0; i--) // Merge the tiles with the same number
                if (MAP[i][j] == MAP[i - 1][j] && MAP[i][j] != 0)
                {
                    didShift = 1;
                    for (int k = i; k >= 0; k--)
                        MAP[k][j] = MAP[k - 1][j];
                    MAP[0][j] = 0;
                    MAP[i][j] *= 2;
                    score += MAP[i][j];
                }
        }
    }
    else if (d == Direction::Right)
    {
        for (int i = 0; i < N; i++) // Every line
        {
            for (int j = M - 1; j >= 0; j--) // Search for every 0 (empty tile) and remove it
                if (MAP[i][j] == 0)
                {
                    bool change = 0;
                    for (int k = j; k > 0; k--) // Shift everything to the right
                    {
                        MAP[i][k] = MAP[i][k - 1];
                        if (MAP[i][k] != 0)
                        {
                            didShift = 1;
                            change = 1;
                        }
                    }
                    MAP[i][0] = 0;
                    if (change) j++; // If something really changed (did not shift only 0s) check again the current position
                }
            for (int j = M - 1; j > 0; j--) // Merge the tiles with the same number
                if (MAP[i][j] == MAP[i][j - 1] && MAP[i][j] != 0)
                {
                    didShift = 1;
                    for (int k = j; k > 0; k--)
                        MAP[i][k] = MAP[i][k - 1];
                    MAP[i][0] = 0;
                    MAP[i][j] *= 2;
                    score += MAP[i][j];
                }
        }
    }
    if (didShift)
        placeNewTile();
}
sf::Music music;

void displayDefeatWindow()
{
    sf::Music fail;
    fail.openFromFile("music/mario fail sound effect.ogg");
    music.openFromFile("music/Crazy Frog - Axel F (Official Video).ogg");
    fail.setVolume(50);
    fail.play();
    music.stop();
    sf::RenderWindow window(sf::VideoMode(250, 250), "Vous avez perdu!");
    sf::Font font;
    sf::Texture lose;
    lose.loadFromFile("image/Fichier 2.png");
    font.loadFromFile("fonts/DancingScript-Bold.ttf");
    sf::Sprite sprite_lose(lose);
    sf::Text text,score_msg;

    score_msg.setFont(font);
    score_msg.setPosition(0, 200);
    score_msg.setFillColor(sf::Color::White);
    score_msg.setString("votre score :" + std::to_string(score));

    while (window.isOpen()) {
        sf::Event event;
        
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
       
        window.draw(sprite_lose);
        window.draw(score_msg);
        window.display();
    }
}


int main()
{
    music.openFromFile("music/Crazy Frog - Axel F (Official Video).ogg");
    music.play();
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode(WIDTH - 8, HEIGHT - 8), "The Heart Finder");

    sf::Texture tblank, t2, t4, t8, t16, t32, t64, t128, t256, t512, t1024, t2048, t4096, t8192 ,win,lose;
    tblank.loadFromFile("image/vide.png");
    t2.loadFromFile("image/deux.png");
    t4.loadFromFile("image/quatre.png");
    t8.loadFromFile("image/huit.png");
    t16.loadFromFile("image/16.png");
    t32.loadFromFile("image/32.png");
    t64.loadFromFile("image/64.png");
    t128.loadFromFile("image/128.png");
    t256.loadFromFile("image/256.png");
    t512.loadFromFile("image/512.png");
    t1024.loadFromFile("image/1024.png");
    t2048.loadFromFile("image/coeur1.png");
    win.loadFromFile("image/win.png");
    lose.loadFromFile("image/1x/Fichier 1.png");

    sf::Sprite sprite_blank(tblank);
    sf::Sprite sprite_2(t2);
    sf::Sprite sprite_4(t4);
    sf::Sprite sprite_8(t8);
    sf::Sprite sprite_16(t16);
    sf::Sprite sprite_32(t32);
    sf::Sprite sprite_64(t64);
    sf::Sprite sprite_128(t128);
    sf::Sprite sprite_256(t256);
    sf::Sprite sprite_512(t512);
    sf::Sprite sprite_1024(t1024);
    sf::Sprite sprite_2048(t2048);
    sf::Sprite sprite_win(win);
    sf::Sprite sprite_lose(lose);

    placeNewTile();
    placeNewTile();

    while (window.isOpen())
    {
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed && !g_win)
            {
                if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A)
                    shift(Direction::Left);
                else if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D)
                    shift(Direction::Right);
                else if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W)
                    shift(Direction::Up);
                else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)
                    shift(Direction::Down);
            }
            else if (event.type == sf::Event::Closed)
                window.close();
        }

        ////////// CHECK IF USER WON THE GAME //////////
        if (!g_win)
            for (int i = 0; i < N; i++)
                for (int j = 0; j < M; j++)
                    if (MAP[i][j] == 2049)
                    {
                        g_win = 1;
                        break;
                    }
        //////check if user lost 
        bool hasMovesLeft = false;

        // Parcourir chaque case du plateau de jeu
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {

                // Vérifier s'il y a des tuiles adjacentes ayant la même valeur
                if (i > 0 && MAP[i][j] == MAP[i - 1][j]) {
                    hasMovesLeft = true;
                }
                if (i < N - 1 && MAP[i][j] == MAP[i + 1][j]) {
                    hasMovesLeft = true;
                }
                if (j > 0 && MAP[i][j] == MAP[i][j - 1]) {
                    hasMovesLeft = true;
                }
                if (j < M - 1 && MAP[i][j] == MAP[i][j + 1]) {
                    hasMovesLeft = true;
                }

                // Si le joueur peut encore faire un mouvement, sortir de la boucle
                if (hasMovesLeft) {
                    break;
                }
            }

            // Si le joueur peut encore faire un mouvement, sortir de la boucle
            if (hasMovesLeft) {
                break;
            }
        }

        // Si le joueur ne peut plus faire de mouvements, afficher un message de défaite
        if (!hasMovesLeft) {
            window.close();
            window.draw(sprite_lose);
            displayDefeatWindow();
            // Ajouter ici du code pour afficher un message et donner la possibilité de rejouer ou quitter le jeu.
        }



    
        ////////// DRAW //////////
        window.clear();
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
            {
                if (MAP[i][j] == 0)
                {
                    sprite_blank.setPosition(j * TILESIZE, i * TILESIZE);
                    window.draw(sprite_blank);
                }
                else if (MAP[i][j] == 2)
                {
                    sprite_2.setPosition(j * TILESIZE, i * TILESIZE);
                    window.draw(sprite_2);
                    
                }
                else if (MAP[i][j] == 4)
                {
                    sprite_4.setPosition(j * TILESIZE, i * TILESIZE);
                    window.draw(sprite_4);
                    
                }
                else if (MAP[i][j] == 8)
                    {
                        sprite_8.setPosition(j * TILESIZE, i * TILESIZE);
                        window.draw(sprite_8);
                        
                    }
                else if (MAP[i][j] == 16)
                {
                    sprite_16.setPosition(j * TILESIZE, i * TILESIZE);
                        window.draw(sprite_16);
                        
                }
                else if (MAP[i][j] == 32)
                {
                    sprite_32.setPosition(j * TILESIZE, i * TILESIZE);
                    window.draw(sprite_32);
                    
                }
                else if (MAP[i][j] == 64)
                {
                    sprite_64.setPosition(j * TILESIZE, i * TILESIZE);
                    window.draw(sprite_64);
                    
                }
                else if (MAP[i][j] == 128)
                {
                    sprite_128.setPosition(j * TILESIZE, i * TILESIZE);
                    window.draw(sprite_128);
                    
                }
                else if (MAP[i][j] == 256)
                {
                    sprite_256.setPosition(j * TILESIZE, i * TILESIZE);
                    window.draw(sprite_256);
                    
                }
                else if (MAP[i][j] == 512)
                {
                    sprite_512.setPosition(j * TILESIZE, i * TILESIZE);
                    window.draw(sprite_512);
                    
                }
                else if (MAP[i][j] == 1024)
                {
                    sprite_1024.setPosition(j * TILESIZE, i * TILESIZE);
                    window.draw(sprite_1024);
                    
                }
                else if (MAP[i][j] == 2048)
                {
                    sprite_2048.setPosition(j * TILESIZE, i * TILESIZE);
                    window.draw(sprite_2048);
                    
                }
            }

        sf::Font font;
        font.loadFromFile("fonts/Poppins-Black.ttf");

        sf::Text text;
        text.setFont(font);
        text.setString("Score: " + std::to_string(score));
        text.setCharacterSize(15);
        text.setFillColor(sf::Color::Black);
        text.setPosition(0, 0);
        //window.clear(sf::Color::White);
        //window.draw(sprite_mb);
        window.draw(text);
        window.display();
    }

    return 0;
}