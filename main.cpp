#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

float sigma = 10;
float rho = 28;
float beta = 8.0 / 3.0;
float h = 0.01f;

float dx( float x, float y ) {
	return sigma * ( y - x );
}

float dy( float x, float y, float z ) {
	return x * ( rho - z ) - y;
}

float dz( float x, float y, float z ) {
	return x * y - beta * z;
}

void rk4( float &x, float &y, float &z ) {
	float kx1 = h * dx( x, y );
	float ky1 = h * dy( x, y, z );
	float kz1 = h * dz( x, y, z );

	float kx2 = h * dx( x + kx1 / 2, y + ky1 / 2 );
	float ky2 = h * dy( x + kx1 / 2, y + ky1 / 2, z + kz1 / 2 );
	float kz2 = h * dz( x + kx1 / 2, y + ky1 / 2, z + kz1 / 2 );

	float kx3 = h * dx( x + kx2 / 2, y + ky2 / 2 );
	float ky3 = h * dy( x + kx2 / 2, y + ky2 / 2, z + kz2 / 2 );
	float kz3 = h * dz( x + kx2 / 2, y + ky2 / 2, z + kz2 / 2 );

	float kx4 = h * dx( x + kx3, y + ky3 );
	float ky4 = h * dy( x + kx3, y + ky3, z + kz3 );
	float kz4 = h * dz( x + kx3, y + ky3, z + kz3 );

	x =  x + ( 1.0 / 6.0 ) * ( kx1 + 2 * kx2 + 2 * kx3 + kx4 );
	y =  y + ( 1.0 / 6.0 ) * ( ky1 + 2 * ky2 + 2 * ky3 + ky4 );
	z =  z + ( 1.0 / 6.0 ) * ( kz1 + 2 * kz2 + 2 * kz3 + kz4 );
}

int main() {
	sf::RenderWindow window( sf::VideoMode( SCREEN_WIDTH, SCREEN_HEIGHT ), "Lorenz Attractor" );
	if ( !ImGui::SFML::Init(window) ) {
		std::cout << "Could not init imgui\n";
		exit( EXIT_FAILURE );
	}

	sf::Clock clock;
	sf::Time dt;

	float x = 0.1;
	float y = 0;
	float z = 0;

	bool play = false;

	window.clear( sf::Color( 14, 26, 37 ) );
	while( window.isOpen() ) {
		sf::Event event;
		while( window.pollEvent( event ) ) {
			ImGui::SFML::ProcessEvent( window, event );
			if( event.type == sf::Event::Closed ) {
				window.close();
			}
		}

		dt = clock.restart();
		ImGui::SFML::Update( window, dt );

		ImGui::Begin( "Menu" );
			if( ImGui::Button( "Play" ) ) {
				play = true;
				x = 0.1; y = 0.0; z = 0.0;
			}
			if( ImGui::Button( "Stop" ) ) {
				play = false;
			}
			ImGui::SliderFloat( "Sigma", &sigma, 0, 40 );
			ImGui::SliderFloat( "Rho", &rho, 0, 40 );
			ImGui::SliderFloat( "Beta", &beta, 0, 40 );
		ImGui::End();

		if( play ) {
			rk4( x, y, z );

			sf::RectangleShape point( sf::Vector2f( 1, 1 ) );
			point.setPosition( x * 10 + SCREEN_WIDTH / 2, - z * 10 + SCREEN_HEIGHT );
			point.setFillColor( sf::Color( 17, 253, 169 ) );
			window.draw( point );
		}

		ImGui::SFML::Render( window );
		window.display();
	}
}