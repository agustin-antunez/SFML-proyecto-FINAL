


#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <string>
using namespace sf;
using namespace std;

int main()
{
	srand(time_t(NULL));


	//Ventana
	RenderWindow ventana(VideoMode(800, 600), "Coppo in the Python's Den");
	ventana.setFramerateLimit(60);
	

	//Fondo

	Sprite cueva;
	Texture txt_cueva;
	txt_cueva.loadFromFile("recursos/cueva.jpg");
	cueva.setTexture(txt_cueva);

	//Audio
	SoundBuffer buffer;
	buffer.loadFromFile("recursos/oof.wav");
	Sound dolor;
	dolor.setVolume(100.f);
	dolor.setBuffer(buffer);
	SoundBuffer buffer2;
	buffer2.loadFromFile("recursos/coin.wav");
	Sound punto;
	punto.setBuffer(buffer2);



	//Texturas del jugador

	CircleShape jugador (30);
	jugador.setRadius(30.f);
	Texture txt_jugador;

	if (!txt_jugador.loadFromFile("recursos/2.png"))
	{
		cout << "ERROR AL CARGAR" << endl;
	}
	jugador.setTexture(&txt_jugador);
	jugador.setPosition(400, 300);
	Vector2f centro;
	int tiempo_diparo = 0;

	//Texturas del disparo

	CircleShape bala (15);
	bala.setRadius(10.f);
	Texture txt_bala;
	if (!txt_bala.loadFromFile("recursos/proyectil.png"))
	{
		cout << "ERROR AL CARGAR" << endl;
	}
	bala.setTexture(&txt_bala);
	vector <CircleShape> balas;
	balas.push_back(CircleShape(bala));


	//Texturas del enemigo

	RectangleShape enemigo;
	enemigo.setSize(Vector2f(80.f, 80.f));
	Texture txt_enemigo;
	if (!txt_enemigo.loadFromFile("recursos/python.png"))
	{
		cout << "ERROR AL CARGAR" << endl;
	}
	enemigo.setTexture(&txt_enemigo);
	vector <RectangleShape> enemigos;
	enemigos.push_back(RectangleShape(enemigo));
	int tiempo_respawn = 0;


	//CRONOMETRO

	Clock reloj;
    //clock.getElapsedTime();
    Time tiempo;
    //timer.asSeconds();
	sf::Text cronometro;
	Font fuente;
	fuente.loadFromFile("recursos/Roboto.ttf");
	cronometro.setFont(fuente);
	cronometro.setString("tiempo");
	cronometro.setCharacterSize(24);
	cronometro.setFillColor(sf::Color::White);


	Clock relojAceleracion;
    //clock.getElapsedTime();
    Time tiempoAceleracion;
    //timer.asSeconds();
	float aceleracion = 1;
	bool flagAceleration = false;


	//Variables
	int vidas = 3;
	int puntos = 0;
	float velocidad_enemigo = 3;
	const float velocidad_jugador = 3;

	//Game Loop
	while (ventana.isOpen())
	{
		Event evento;
		while (ventana.pollEvent(evento))
		{
			if (evento.type == Event:: Closed || Keyboard::isKeyPressed(Keyboard::Escape))
				ventana.close();
		}
		
		//Tiempo Ronda

		tiempo = reloj.getElapsedTime();
		int num = (int)tiempo.asSeconds();
		//cout << "tiempo: " << num << endl;
		string strTiempo = to_string(num);
		string text = "Tiempo de la ronda: "+strTiempo; 
		cronometro.setString(text);
		//Actualizaciones 

		//Vidas 
		
		Text vida;
		vida.setFont(fuente);
		string strVidas = to_string(vidas);
		string texto_vidas = "Vidas: " + strVidas;
		vida.setString(texto_vidas);
		vida.setCharacterSize(24);
		vida.setFillColor(Color::White);
		vida.setPosition({ 0, 40 });

		//Puntos 
		
		Text puntaje;
		puntaje.setFont(fuente);
		string strPuntaje = to_string(puntos);
		string texto_puntaje = "Puntaje: " + strPuntaje;
		puntaje.setString(texto_puntaje);
		puntaje.setCharacterSize(24);
		puntaje.setFillColor(Color::White);
		puntaje.setPosition({ 0 , 60 });
	
		//Aceleracion de enemigos

		tiempoAceleracion = relojAceleracion.getElapsedTime();
		if((int)tiempoAceleracion.asSeconds() % 10 == 0){
			if(flagAceleration==false)
			{
				velocidad_enemigo = velocidad_enemigo + 0.5f;
			}
			flagAceleration =true;	
		}
		else
		{
			flagAceleration = false;
		}
		cout << velocidad_enemigo << endl;
		//cout << tiempoAceleracion.asSeconds() << endl;
		centro = Vector2f(jugador.getPosition().x + jugador.getRadius(), jugador.getPosition().y + jugador.getRadius());

		//Eventos del jugador

		if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up))
		{
			jugador.move(0, -velocidad_jugador);
		}
		if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down))
		{
			jugador.move(0, velocidad_jugador);
		}
		if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
		{
			jugador.move(-velocidad_jugador, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
		{
			jugador.move(velocidad_jugador, 0);
		}

		// Colision contra la pantalla para que no se salga de ella

		if (jugador.getPosition().x < 25) {
			jugador.setPosition(25, jugador.getPosition().y);
		}
		if (jugador.getPosition().y < 65) {
			jugador.setPosition(jugador.getPosition().x, 65);
		}
		if (jugador.getPosition().x + jugador.getGlobalBounds().width > 800) {
			jugador.setPosition(800 - jugador.getGlobalBounds().width, jugador.getPosition().y);
		}
		if (jugador.getPosition().y + jugador.getGlobalBounds().width > 530) {
			jugador.setPosition(jugador.getPosition().x, 530 - jugador.getGlobalBounds().width);
		}


		//eventos disparo

		if (tiempo_diparo < 30)
			tiempo_diparo++;

		if (Mouse::isButtonPressed(Mouse::Left) && tiempo_diparo >= 30)
		{
			bala.setPosition(centro);
			balas.push_back(CircleShape(bala));

			tiempo_diparo = 0;
		}

		for (size_t i = 0; i < balas.size(); i++)
		{
			balas[i].move(10.f, 0.f);
			if (balas[i].getPosition().x < 0)
				balas.erase(balas.begin()+ i);
		}


		//Spawn de enemigos 

		if (tiempo_respawn < 100)
			tiempo_respawn++;


		if (tiempo_respawn >= 100)
		{
			enemigo.setPosition(1000.f, (rand() % int(ventana.getSize().y - 200) + enemigo.getSize().y));
			enemigos.push_back(RectangleShape(enemigo));
			tiempo_respawn = 0;
		}

		for (int i = 0; i < enemigos.size(); i++)
		{
			enemigos[i].move(-velocidad_enemigo, 0.f);

		}
		//Colisiones
		
		//Colision con personaje y Colision con enemigo

		for (size_t j = 0; j < enemigos.size(); j++)
		{
			if (jugador.getGlobalBounds().intersects(enemigos[j].getGlobalBounds()))
			{
				enemigos.erase(enemigos.begin() + j);
				vidas--;
				jugador.setFillColor(Color::Red);
				dolor.play();
			}
			else
				jugador.setFillColor(Color::White);

		}

		//Colision de las balas con el enemigo

		for (size_t i = 0; i < balas.size(); i++)
		{
			for (size_t j = 0; j < enemigos.size(); j++)
			{
				if (balas[i].getGlobalBounds().intersects(enemigos[j].getGlobalBounds()))
				{
					puntos++;
					punto.play();
					balas.erase(balas.begin() + i);
					enemigos.erase(enemigos.begin() + j);	

				}
			}
		}
	
		//draw

		ventana.clear();

		ventana.draw(cueva);
		ventana.draw(cronometro);
		ventana.draw(vida);
		ventana.draw(puntaje);
		ventana.draw(jugador);
	
		for (int i = 0; i < enemigos.size(); i++)
		{
			ventana.draw(enemigos[i]);
		}

		for (size_t i = 0; i < balas.size(); i++)
		{
			ventana.draw(balas[i]);
		}
		
		ventana.display();
	}

	return 0;
}