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

void Jogo::iniciar(){

    int rodada{0};

    do{
        rodada++;
        executarRodada( rodada );

    }while( !jogoEncerrado );
    
    
};

void Jogo::imprimirStatusMesa() const {
    std::cout << mesa.getSaldo() << std::endl;
};

void Jogo::executarRodada( const int numeroRodada ){

    int pote{0};

    for( Jogador j : jogadores ){
        std::cout << j.getNome() << " "  << j.getSaldo() << std::endl;
    }
    imprimirStatusMesa();

    for( Jogador j : jogadores ){
        j.realizarAposta( solicitarAposta( j, 1 ) );
        pote += j.getApostaRodadaAtual();
    }

    for( Jogador j : jogadores ){
        if( j.isAtivoNaRodada( ) ){

            char jogada;
            std::cout << numeroRodada << "° Rodada" << std::endl;
            std::cout << j.getNome() << " faca sua jogada: ";
            std::cin >> jogada;

            switch ( jogada )
            {
            case 'r': // rock = pedra
                j.setJogadaAtual( Jogada::PEDRA );
                break;
            case 'p': // paper = papel
                j.setJogadaAtual( Jogada::PAPEL );
            case 's': // scissors = tesoura
                j.setJogadaAtual( Jogada::TESOURA);
            default:
                std::cerr << "VALOR DIGITADO INVALIDO!!!";
                break;
            }
        }
    }

};
std::vector<Jogador> Jogo::determinarVencedores( std::vector<Jogador> &ativos ){

};