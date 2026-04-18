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
    std::vector<Jogador> ativos;
    std::vector<Jogador> vencedores;

    for( Jogador &j : jogadores ){
        if( j.isAtivoNaRodada() ){
            std::cout << j.getNome() << " "  << j.getSaldo() << std::endl;
            ativos.push_back( j );
        } 
    }
    imprimirStatusMesa();

    for( Jogador &j : ativos ){
        j.realizarAposta( solicitarAposta( j, 1 ) );
        pote += j.getApostaRodadaAtual();
    }

    for( Jogador &j : ativos ){
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

    vencedores = determinarVencedores( ativos );


};
std::vector<Jogador> Jogo::determinarVencedores( std::vector<Jogador> &ativos ){

    bool pedra{ false };
    bool papel{ false };
    bool tesoura{ false };
    Jogada jogadaVencedora;
    std::vector<Jogador> vencedores{};

    for( Jogador j : ativos ){
        switch ( j.getJogadaAtual() )
        {
        case Jogada::PEDRA :
            pedra = true;
            break;
        case Jogada::PAPEL :
            papel = true;
            break;
        case Jogada::TESOURA :
            tesoura = true;
            break;
        default:
            break;
        }
    }

    if( pedra && papel && tesoura ){
        return vencedores;
    } else if( pedra && papel ){
        jogadaVencedora = Jogada::PAPEL;
    } else if( papel && tesoura ){
        jogadaVencedora = Jogada::TESOURA;
    } else if( tesoura && pedra ) {
        jogadaVencedora = Jogada::PEDRA;
    } else{
        return vencedores;
    }

    for( Jogador &j : ativos ){
        if( j.getJogadaAtual() == jogadaVencedora ){
            vencedores.push_back( j );
        }
    }

    return vencedores;

};