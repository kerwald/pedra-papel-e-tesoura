#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include "Jogador.hpp"
#include "Jogo.hpp"

Jogo::Jogo( std::vector<Jogador> &jogadores ) : mesa( 0 ), jogadores( jogadores ), empate(0) {

    int numDeJogadores;
    do{
        std::cout << "Digite o numero de jogadores ( 2 - 6 ) : ";
        std::cin >> numDeJogadores;
    } while( numDeJogadores < 2 || numDeJogadores > 6 );

    for( int i=1; i<=numDeJogadores; i++ ){
        std::string nome;
        std::cout << "jogador numero " << i << " digite seu nome: ";
        std::cin >> nome;
        Jogador jogador( nome );
        jogador.adicionarSaldo( 10 );
        jogadores.push_back( jogador );
    }

    int valorInicial = (int) std::ceil( (double) ( numDeJogadores * 10 ) / 2.0 );
    this->mesa.adicionarSaldo( valorInicial );
    iniciar();
}

void Jogo::iniciar(){

    int rodada{0};

    do{
        rodada++;
        executarRodada( rodada );
        if( contarJogadoresComSaldo() < 2 ){
            jogoEncerrado = true;
            declararCampeaoFinal();
        }

    }while( !jogoEncerrado );
    
    
}

void Jogo::executarRodada( const int numeroRodada ){

    int pote{0};
    std::vector<Jogador*> ativos;
    std::vector<Jogador*> vencedores;
    
    for( Jogador &j : jogadores ){
        if( j.getSaldo() > 0 ){
            std::cout << j.getNome() << " "  << j.getSaldo() << std::endl;
            j.setAtivoNaRodada( true );
            ativos.push_back( &j );
        } 
    }

    imprimirStatusMesa();

    for( Jogador* &j : ativos ){
        j->realizarAposta( solicitarAposta( *j, 1 ) );
        pote += j->getApostaRodadaAtual();
    }

    coletarJogadasOcultas( ativos );

    vencedores = determinarVencedores( ativos );

    if( vencedores.size() == 0 ){
        empate++;
        if( empate == LIMITE_EMPATES ){
            aplicarPenalidadesLimiteEmpates( ativos, pote );
            return;
        }
        pote = executarSubRodadaDesempate( ativos, pote );
    } else{
        distribuirPremio( ativos, vencedores, pote );
        empate = 0;
    }

}

void Jogo::imprimirStatusMesa() const {
    std::cout << mesa.getSaldo() << std::endl;
}

int Jogo::solicitarAposta( Jogador &j, const int apostaMinima ){

    int aposta{0};
    do{
        std::cout << "Valor minimo: " << apostaMinima << std::endl;
        std::cout << j.getNome() << " faca sua aposta: ";
        std::cin >> aposta;
    }while( aposta < apostaMinima );

    return aposta;

}

void Jogo::coletarJogadasOcultas( std::vector<Jogador*> &ativos ){

    for( Jogador* &j : ativos ){
        if( j->isAtivoNaRodada( ) ){

            char jogada;
            std::cout << " r -> pedra, p -> papel, s -> tesoura " << std::endl;
            std::cout << j->getNome() << " faca sua jogada: ";
            std::cin >> jogada;

            switch ( jogada )
            {
            case 'r': // rock = pedra
                j->setJogadaAtual( Jogada::PEDRA );
                break;
            case 'p': // paper = papel
                j->setJogadaAtual( Jogada::PAPEL );
                break;
            case 's': // scissors = tesoura
                j->setJogadaAtual( Jogada::TESOURA);
                break;
            default:
                std::cerr << "VALOR DIGITADO INVALIDO!!!";
                break;
            }
        }
        limparConsole();
    }
}

void Jogo::limparConsole() const{
    system("cls");
    for( Jogador &j : jogadores ){
        if( j.isAtivoNaRodada() ){
            std::cout << j.getNome() << " "  << j.getSaldo() << std::endl;
        } 
    }

    imprimirStatusMesa();

}

int Jogo::executarSubRodadaDesempate( std::vector<Jogador*> &ativos, int pote ){

    bool continuar{ true };
    int devolucaoJogador{0};
    int devolucaoMesa{0};
    std::vector<Jogador*> continuamNaRodada;

    for( Jogador* &j : ativos ){
        if( j->getSaldo() == 0 ){
            std::cout << "all-in!!" << std::endl;
        } else{
            std::cout << j->getNome() << " por favor digite 1 para continuar ou 0 para desistir da rodada: ";
            std::cin >> continuar;
            if( continuar ){
                continuamNaRodada.push_back( j );
                j->reduzirSaldo( 1 );
                j->realizarAposta( j->getApostaRodadaAtual() + 1  );
                pote++;
            }else{
                devolucaoJogador = (int) std::ceil( (double) ( j->getApostaRodadaAtual() ) / 2.0 );
                devolucaoMesa = j->getApostaRodadaAtual() - devolucaoJogador;
                pote -= j->getApostaRodadaAtual();
                j->adicionarSaldo( devolucaoJogador );
                j->setAtivoNaRodada( false );
                mesa.adicionarSaldo( devolucaoMesa );
            }
        }   
    }
    ativos.clear();
    ativos = continuamNaRodada;
    if( ativos.size() > 1 ){
        coletarJogadasOcultas( ativos );
    }
    std::vector<Jogador*> vencedores = determinarVencedores( ativos );
    if( vencedores.size() == 0 ){
        empate++;
        if( empate == LIMITE_EMPATES ){
            aplicarPenalidadesLimiteEmpates( ativos, pote );
            return pote;
        }
        pote = executarSubRodadaDesempate( ativos, pote );
    } else{
        distribuirPremio( ativos, vencedores, pote );
        empate = 0;
    }
    return pote;
}

std::vector<Jogador*> Jogo::determinarVencedores( std::vector<Jogador*> &ativos ){

    bool pedra{ false };
    bool papel{ false };
    bool tesoura{ false };
    Jogada jogadaVencedora;
    std::vector<Jogador*> vencedores{};

    if( ativos.size() == 1 ){
        vencedores.push_back( ativos[0] );
        return vencedores;
    }

    for( Jogador* &j : ativos ){
        switch ( j->getJogadaAtual() )
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

    for( Jogador* &j : ativos ){
        if( j->getJogadaAtual() == jogadaVencedora ){
            vencedores.push_back( j );
        }
    }

    return vencedores;

}

void Jogo::distribuirPremio( std::vector<Jogador*> &ativos, std::vector<Jogador*> &vencedores, int &pote ){

    int totalDisponivel = pote + mesa.getSaldo();
    int totalApostadoVencedores{0};
    int totalPremioVencedores{0};
    for( Jogador* &j : vencedores ){
        totalApostadoVencedores += j->getApostaRodadaAtual();
    }
    totalPremioVencedores = totalApostadoVencedores * 2;

    if( pote >= totalPremioVencedores ){
        for( Jogador* &j : vencedores ){
            j->adicionarSaldo( j->getApostaRodadaAtual()*2 );
            pote -= j->getApostaRodadaAtual()*2;
        }

    } else if ( totalDisponivel >= totalPremioVencedores ){
        for( Jogador* &j : vencedores ){
            int diferenca{ ( j->getApostaRodadaAtual()*2 ) - pote };
            if( diferenca <= 0 ){
                j->adicionarSaldo( j->getApostaRodadaAtual()*2 );
                pote -= j->getApostaRodadaAtual()*2;
            }else{
                j->adicionarSaldo( j->getApostaRodadaAtual()*2 );
                mesa.reduzirSaldo( diferenca );
                pote = 0;
            }
        }
    } else{
        mesa.adicionarSaldo( pote );
        pote = 0;
        int parteProporcional{0};
        int resto{ totalDisponivel % totalApostadoVencedores };
        for( Jogador* &j : vencedores ){
            parteProporcional = ( totalDisponivel * j->getApostaRodadaAtual()) / totalApostadoVencedores;
            j->adicionarSaldo( parteProporcional );
            mesa.reduzirSaldo( parteProporcional );
            if( resto > 0 ){
                j->adicionarSaldo( 1 );
                mesa.reduzirSaldo( 1 );
                resto--;
            }
        }

        jogoEncerrado = true;
        declararCampeaoFinal();

    }

    if (pote > 0) {
            mesa.adicionarSaldo( pote );
            pote = 0;
    }

}

void Jogo::aplicarPenalidadesLimiteEmpates( std::vector<Jogador*> &ativos, int pote ){
    for( Jogador* &j : ativos ){
        pote -= j->getApostaRodadaAtual();
        j->adicionarSaldo( j->getApostaRodadaAtual() - 1 );
        mesa.adicionarSaldo(1);
    }
}

int Jogo::contarJogadoresComSaldo() const{
    int totalJogadoresComSaldo{0};
    for( Jogador &j : jogadores){
        if( j.getSaldo() >  0 ){
            totalJogadoresComSaldo++;
        }
    }
    return totalJogadoresComSaldo;
}

void Jogo::declararCampeaoFinal(){

    Jogador* campeao = nullptr;

    int saldo{0};
    for( Jogador &j : jogadores ){
        if( saldo < j.getSaldo() ){
            saldo = j.getSaldo();
            campeao = &j;
        }
    }

    if( campeao != nullptr ){
        std::cout << "O jogador " << campeao->getNome() <<  " é o grande campeao!!!" << std::endl;
    } else{
        std::cout << "Jogo finalizado sem campeao!!! " << std::endl;
    }
    
}