#include <iostream>
#include <vector>
#include <cmath>
#include "Jogador.hpp"
#include "Jogo.hpp"

Jogo::Jogo( std::vector<Jogador> &jogadores ) : mesa( 0 ), jogadores( jogadores ) {

    int numDeJogadores;
    do{
        std::cin >> numDeJogadores;
    } while( numDeJogadores < 2 || numDeJogadores > 6 );

    for( int i=0; i<numDeJogadores; i++ ){
        std::string nome;
        std::cin >> nome;
        Jogador jogador( nome );
        jogador.adicionarSaldo( 10 );
        jogadores.push_back( jogador );
    }

    int valorInicial = (int) std::ceil( (double) ( numDeJogadores * 10 ) / 2.0 );
    this->mesa.adicionarSaldo( valorInicial );
    iniciar();
};