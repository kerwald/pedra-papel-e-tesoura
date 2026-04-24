#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <thread>
#include <random>
#include "Jogador.hpp"
#include "Jogo.hpp"

Jogo::Jogo( std::vector<Jogador> &jogadores ) : mesa( 0 ), jogadores( jogadores ), empate(0), faseAtual( Fase::ESPERA ), jogadoresProntos(0), pote(0), jogoEncerrado( false ) {

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
        jogador.setJogo( this );
        jogadores.push_back( jogador );
    }

    int valorInicial = (int) std::ceil( (double) ( numDeJogadores * 10 ) / 2.0 );
    this->mesa.adicionarSaldo( valorInicial );
    iniciar();
}

void Jogo::iniciar(){

    for (Jogador& j : jogadores) {
        threads.emplace_back( &Jogador::run, &j );
    }

    int rodada{0};

    do{
        rodada++;
        executarRodada( rodada );
        if( contarJogadoresComSaldo() < 2 ){
            encerrarJogo();
            declararCampeaoFinal();
        }

    }while( !isEncerrado() );   

    for ( std::thread& t : threads ) {
        if ( t.joinable() ) {
            t.join();
        }
    }   
    
}

void Jogo::executarRodada( const int numeroRodada ){

    zerarPote();
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

    mudarFase( Fase::APOSTA );
    esperarTodosTerminarem();

    mudarFase( Fase::JOGADA );
    esperarTodosTerminarem();

    vencedores = determinarVencedores( ativos );

    if( vencedores.size() == 0 ){
        empate++;
        if( empate == LIMITE_EMPATES ){
            aplicarPenalidadesLimiteEmpates( ativos );
            return;
        }
        executarSubRodadaDesempate( ativos );
    } else{
        distribuirPremio( ativos, vencedores );
        empate = 0;
    }

}

void Jogo::imprimirStatusMesa() const {
    std::cout << mesa.getSaldo() << std::endl;
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

void Jogo::executarSubRodadaDesempate( std::vector<Jogador*> &ativos ){

    int devolucaoJogador{0};
    int devolucaoMesa{0};
    std::vector<Jogador*> continuamNaRodada;
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> distr(0, 1); 
    bool continuar;

    for( Jogador* &j : ativos ){
        if( j->getSaldo() == 0 ){
            std::cout << j->getNome() << " continua em all-in!!" << std::endl;
            continuamNaRodada.push_back( j );
        } else{
            std::cout << j->getNome() << " por favor digite 1 para continuar ou 0 para desistir da rodada: ";
            continuar = (bool) distr(gen);
            if( continuar ){
                continuamNaRodada.push_back( j );
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

    mudarFase( Fase::AUMENTARAPOSTA );
    esperarTodosTerminarem();

    if( ativos.size() > 1 ){
        mudarFase( Fase::JOGADA );
        esperarTodosTerminarem();
    }

    std::vector<Jogador*> vencedores = determinarVencedores( ativos );
    if( vencedores.size() == 0 ){
        empate++;
        if( empate == LIMITE_EMPATES ){
            aplicarPenalidadesLimiteEmpates( ativos );
            return;
        }
        executarSubRodadaDesempate( ativos );
    } else{
        distribuirPremio( ativos, vencedores );
        empate = 0;
    }
    return;
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

void Jogo::distribuirPremio( std::vector<Jogador*> &ativos, std::vector<Jogador*> &vencedores ){

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
                zerarPote();
            }
        }
    } else{
        mesa.adicionarSaldo( pote );
        zerarPote();
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

        encerrarJogo();
        std::cout << "Mesa nao tem como pagar, jogo encerrado!!! ";
        declararCampeaoFinal();

    }

    if (pote > 0) {
            mesa.adicionarSaldo( pote );
            zerarPote();
    }

}

void Jogo::aplicarPenalidadesLimiteEmpates( std::vector<Jogador*> &ativos ){
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
        std::cout << " O jogador " << campeao->getNome() <<  " e o grande campeao!!!" << std::endl;
    } else{
        std::cout << " Jogo finalizado sem campeao!!! " << std::endl;
    }
    
}
void Jogo::mudarFase( Fase novaFase ){
    {
        std::lock_guard<std::mutex> lock(mtx);
        faseAtual = novaFase;
        jogadoresProntos = 0;
    }
    cv.notify_all();
}

void Jogo::jogadorTerminouFase(){
    std::lock_guard<std::mutex> lock(mtx);
    jogadoresProntos++;
    cv.notify_all();
}

void Jogo::esperarTodosTerminarem(){
    std::unique_lock<std::mutex> lock( mtx );
    cv.wait(lock, [&] {
        return jogadoresProntos == jogadores.size();
    });
}

int Jogo::getPote(){
    std::lock_guard<std::mutex> lock( mtx );
    return pote;
}

void Jogo::aumentarPote( int valor ){
    std::lock_guard<std::mutex> lock( mtx );
    pote += valor;
}

void Jogo::zerarPote(){
    std::lock_guard<std::mutex> lock( mtx );
    pote = 0;
}

Fase Jogo::esperarProximaFase( Fase ultimaFase ) {

    std::unique_lock<std::mutex> lock( mtx );

    cv.wait( lock, [&] {
        return faseAtual != ultimaFase || jogoEncerrado;
    });

    return faseAtual;
}

void Jogo::encerrarJogo() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        jogoEncerrado = true;
        faseAtual = Fase::ENCERRADO;
    }
    cv.notify_all();
}

bool Jogo::isEncerrado() {
    std::lock_guard<std::mutex> lock(mtx);
    return jogoEncerrado;
}