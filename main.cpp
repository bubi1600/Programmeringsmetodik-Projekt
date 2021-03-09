#include"class.h"

using namespace sf;

int main()
{
	srand(time(NULL));

	RenderWindow window(VideoMode(800, 600), "Spaceship game!", Style::Default);
	window.setFramerateLimit(60);

	//Font
	Font font;
	font.loadFromFile("Fonts/Masked Hero.ttf");

	//Textures
	Texture playerTex;
	playerTex.loadFromFile("Textures/ship.png");

	Texture enemyTex;
	enemyTex.loadFromFile("Textures/enemy.png");

	Texture bulletTex;
	bulletTex.loadFromFile("Textures/missileTex01.png");

	//Set UI
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(20);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(10.f, 10.f);

	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(30);
	gameOverText.setFillColor(Color::Red);
	gameOverText.setPosition(250.f, window.getSize().y / 2);
	gameOverText.setString("GAME OVER!");

	//Set Player
	int score = 0;
	Player player(&playerTex);
	int shootTimer = 0;
	Text hpText;
	hpText.setFont(font);
	hpText.setCharacterSize(12);
	hpText.setFillColor(Color::White);

	//Set Enemy
	int enemySpawnTimer = 0;
	std::vector<Enemy> enemies;
	Text eHpText;
	eHpText.setFont(font);
	eHpText.setCharacterSize(12);
	eHpText.setFillColor(Color::White);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		if (player.HP > 0)
		{
			//Player
			if (Keyboard::isKeyPressed(Keyboard::A))
				player.shape.move(-10.f, 0.f);
			if (Keyboard::isKeyPressed(Keyboard::D))
				player.shape.move(10.f, 0.f);

			//hp pos
			hpText.setPosition(player.shape.getPosition().x, player.shape.getPosition().y - 15);
			hpText.setString(std::to_string(player.HP) + "/" + std::to_string(player.HPMax));

			/**
			 * Collision with window bounds.
			 * stop the player from going outside the window
			 *
			 */
			if (player.shape.getPosition().x <= 0) //Left
				player.shape.setPosition(0.f, player.shape.getPosition().y);
			if (player.shape.getPosition().x >= window.getSize().x - player.shape.getGlobalBounds().width) //Right
				player.shape.setPosition(window.getSize().x - player.shape.getGlobalBounds().width, player.shape.getPosition().y);

			//Update Controls
			if (shootTimer < 15)
				shootTimer++;

			if (Keyboard::isKeyPressed(Keyboard::Space) && shootTimer >= 15) //Shooting
			{
				player.bullets.push_back(Bullet(&bulletTex, player.shape.getPosition()));
				shootTimer = 1; //reset timer
			}

			//Bullets
			for (size_t i = 0; i < player.bullets.size(); i++)
			{
				//Move
				player.bullets[i].shape.move(0.f, -20.f);

				//Out of window bounds
				if (player.bullets[i].shape.getPosition().x > window.getSize().x)
				{
					player.bullets.erase(player.bullets.begin() + i);
					break;
				}

				//Enemy collision
				for (size_t k = 0; k < enemies.size(); k++)
				{
					if (player.bullets[i].shape.getGlobalBounds().intersects(enemies[k].shape.getGlobalBounds()))
					{
						if (enemies[k].HP <= 1)
						{
							score += enemies[k].HPMax;
							enemies.erase(enemies.begin() + k);
						}
						else
							enemies[k].HP--; //ENEMY TAKE DAMAGE

						player.bullets.erase(player.bullets.begin() + i);
						break;
					}
				}
			}
			//Enemy
			if (enemySpawnTimer < 25)
				enemySpawnTimer++;

			//enemy spawn
			if (enemySpawnTimer >= 25)
			{
				enemies.push_back(Enemy(&enemyTex, window.getSize()));
				enemySpawnTimer = 0; //reset timer
			}

			for (size_t i = 0; i < enemies.size(); i++)
			{
				enemies[i].shape.move(0.f, 6.f);

				if (enemies[i].shape.getPosition().x <= 0 - enemies[i].shape.getGlobalBounds().width)
				{
					enemies.erase(enemies.begin() + i);
					break;
				}

				if (enemies[i].shape.getGlobalBounds().intersects(player.shape.getGlobalBounds()))
				{
					enemies.erase(enemies.begin() + i);

					player.HP--; // PLAYER TAKE DAMAGE
					break;
				}
			}

			//UI Update
			scoreText.setString("Score: " + std::to_string(score));
		}

		//Draw 
		window.clear();

		//player
		window.draw(player.shape);

		//Bullets
		for (size_t i = 0; i < player.bullets.size(); i++)
		{
			window.draw(player.bullets[i].shape);
		}

		/**
		 * Draw out all enemies
		 * and hp text.
		 *
		 */
		for (size_t i = 0; i < enemies.size(); i++)
		{
			eHpText.setString(std::to_string(enemies[i].HP) + "/" + std::to_string(enemies[i].HPMax));
			eHpText.setPosition(enemies[i].shape.getPosition().x, enemies[i].shape.getPosition().y - 15);
			window.draw(eHpText);
			window.draw(enemies[i].shape);
		}

		//UI
		window.draw(scoreText);
		window.draw(hpText);

		if (player.HP <= 0)
			window.draw(gameOverText);

		window.display();
	}

	return 0;
}