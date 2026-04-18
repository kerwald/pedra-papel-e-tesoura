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

void Jogo::executarRodada( const int numeroRodada ){

    int pote{0};
    int empate{0};
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

    if( vencedores.size() == 0 ){
        empate++;
    } else{
        distribuirPremio( ativos, vencedores, pote );
    }

};

void Jogo::imprimirStatusMesa() const {
    std::cout << mesa.getSaldo() << std::endl;
};

int Jogo::solicitarAposta( Jogador &j, const int apostaMinima ){

    int aposta{0};
    do{
        std::cout << "Valor minimo: " << apostaMinima << std::endl;
        std::cout << j.getNome() << " faça sua aposta: ";
        std::cin >> aposta;
    }while( aposta < apostaMinima );

    return aposta;

}

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

void Jogo::distribuirPremio( std::vector<Jogador> ativos, std::vector<Jogador> vencedores, int &pote ){

    int total = pote + mesa.getSaldo();
    int valorRequisitado{0};
    for( Jogador &j : vencedores ){
        valorRequisitado += j.getApostaRodadaAtual()*2;
    }

    if( pote >= valorRequisitado ){
        for( Jogador &j : vencedores ){
            j.adicionarSaldo( j.getApostaRodadaAtual()*2 );
            pote -= j.getApostaRodadaAtual()*2;
        }
        mesa.adicionarSaldo( pote );
        pote = 0;

    } else if ( total >= valorRequisitado ){
        

    } else{

        int divisaoIgualitária{ total / vencedores.size() };
        int restoDaDivisão{ total % vencedores.size() };
        mesa.adicionarSaldo( pote );
        pote = 0;
        for( Jogador &j : vencedores ){
            j.adicionarSaldo( divisaoIgualitária );
            mesa.reduzirSaldo( divisaoIgualitária );
            if( restoDaDivisão != 0 ){
                j.adicionarSaldo( 1 );
                mesa.reduzirSaldo( 1 );
                restoDaDivisão--;
            }
        }

        jogoEncerrado = true;

    }

};