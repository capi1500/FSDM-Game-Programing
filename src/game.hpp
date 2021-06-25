//
// Created by Kacper on 12/03/2021.
//

#ifndef PROGRAMOWANIEGIER_GAME_HPP
#define PROGRAMOWANIEGIER_GAME_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <finiteStateMachine/finiteStateMachine.hpp>
#include <systems/scene.hpp>
#include <map/map.hpp>
#include "gameEvent.hpp"
#include "statistics.hpp"

class Game : public Listener<GameEvent>, public Listener<sf::Event>{
	private:
		sf::RenderWindow window;
		sf::View view;
		FiniteStateMachine scenes;
		Statistics* statistics;
		
		bool active;
		
		Scene* getScene();
		
		Map map;
	public:
		void run();
		
		Scene* newScene();
		
		void onNotify(const GameEvent& event) override;
		void onNotify(const sf::Event& event) override;
		
		Game();
		~Game();
};

#endif //PROGRAMOWANIEGIER_GAME_HPP
