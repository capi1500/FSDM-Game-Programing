//
// Created by Kacper on 12/03/2021.
//

#include "game.hpp"
#include <systems/inputHandler.hpp>
#include <systems/assetManager.hpp>
#include <entities/pacman.hpp>
#include <iostream>
#include <entities/ghost.hpp>
#include <systems/console.hpp>
#include <entities/fruit.hpp>

Scene* Game::getScene(){
	return static_cast<Scene*>(scenes.get());
}

Game::Game(){
	AssetManager::get().load();
	
	window.create(sf::VideoMode(12 * 28, 12 * 40), "Pacman");
	window.setFramerateLimit(60);
	
	InputHandler::get().registerWindow(&window);
	active = true;
	gameEventSignal.addListener(this);
	InputHandler::get().addListener(this);
	
	scenes.add(newScene());
	
	statistics = new Statistics();
	
	view = sf::View(sf::FloatRect(36, -36, 12 * 28, 12 * 40));
	long double width = static_cast<long double>(28 * window.getSize().y) / static_cast<long double>(40 * window.getSize().x);
	view.setViewport(sf::FloatRect((1 - width) / 2, 0, width, 1));
	window.setView(view);
	
	window.setVerticalSyncEnabled(true);
}

Game::~Game(){
	gameEventSignal.removeListener(this);
	scenes.clear();
}

void Game::run(){
	Console console;
	//console.listenType(Message::Debug);
	
	gameEventSignal.addListener(statistics);
	
	Scene* scene;
	sf::Time time;
	sf::Clock clock;
	while(active){
		time = clock.restart();
		
		scene = getScene();
		
		InputHandler::get().handleEvents();
		
		scene->update(time);
		
		gameEventSignal.handleEvents();
		message.handleEvents();
		
		window.clear();
		window.draw(map);
		window.draw(*scene);
		window.draw(*statistics);
		window.display();
	}
	gameEventSignal.removeListener(statistics);
	
	window.close();
}

void Game::onNotify(const GameEvent& event){
	if(statistics->getPointsEaten() * 2 == statistics->getPointsTotal() + 1){
		Fruit* fruit = new Fruit(static_cast<Fruit::Type>((statistics->getLevel() - 1) % Fruit::Count), sf::Vector2u(34, 47));
		gameEventSignal.addListener(fruit);
		getScene()->addEntity(fruit);
	}
	if(event.type == GameEvent::Closed)
		active = false;
	if(event.type == GameEvent::NextLevel){
		Scene* scene = newScene();
		scenes.replace(scene);
	}
}

void Game::onNotify(const sf::Event& event){
	if(event.type == sf::Event::Resized){
		long double width = static_cast<long double>(28 * event.size.height) / static_cast<long double>(40 * event.size.width);
		view.setViewport(sf::FloatRect((1 - width) / 2, 0, width, 1));
		window.setView(view);
	}
}

Scene* Game::newScene(){
	Scene* scene = new Scene(scenes);
	scene->addEntity(new Pacman(map));
	scene->addEntity(new Ghost(map, {34, 28}, Ghost::Follow));
	scene->addEntity(new Ghost(map, {30, 30}, Ghost::Ambush));
	scene->addEntity(new Ghost(map, {38, 30}, Ghost::Corner));
	scene->addEntity(new Ghost(map, {34, 30}, Ghost::Mixed));
	return scene;
}
