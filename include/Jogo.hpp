#pragma once
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "Fase.hpp"
#include "Jogador.hpp"
#include "Mesa.hpp"

class Jogo{
    private:
        std::vector<Jogador> &jogadores;
        std::vector<std::thread> threads;
        Mesa mesa;
        int pote;
        Fase faseAtual;
        std::mutex mtx;
        std::condition_variable cv;
        int jogadoresProntos;
        bool  jogoEncerrado;
        int empate;
        const int LIMITE_EMPATES{ 5 };

    public:
        Jogo( std::vector<Jogador> &jogadores );
        void iniciar( );
        void executarRodada( const int numeroRodada );
        void imprimirStatusMesa() const;
        void limparConsole() const;
        int executarSubRodadaDesempate( std::vector<Jogador*> &ativos, int pote );
        std::vector<Jogador*> determinarVencedores( std::vector<Jogador*> &ativos );
        void distribuirPremio( std::vector<Jogador*> &ativos, std::vector<Jogador*> &vencedores, int &pote );
        void aplicarPenalidadesLimiteEmpates( std::vector<Jogador*> &ativos, int pote );
        int contarJogadoresComSaldo() const;
        void declararCampeaoFinal();
        Fase esperarProximaFase();
        void mudarFase( Fase novaFase );
        void esperarTodosTerminarem();
        void jogadorTerminouFase();
        bool isEncerrado();
        int getPote();
        void aumentarPote( int valor );

};