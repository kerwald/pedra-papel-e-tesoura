#pragma once

#ifndef JOGO_HPP
#define JOGO_HPP

#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "Fase.hpp"
#include "Mesa.hpp"
#include <random>

// FORWARD DECLARATION: Avisa ao compilador que 'Jogador' existe, 
// sem precisar ler o arquivo .hpp dele agora.
class Jogador;

class Jogo{
    private:
        std::vector<Jogador> &jogadores;
        std::vector<std::thread> threads;
        std::mt19937 gen;
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
        void imprimirStatus() const;
        void executarSubRodadaDesempate( std::vector<Jogador*> &ativos );
        std::vector<Jogador*> determinarVencedores( std::vector<Jogador*> &ativos );
        void distribuirPremio( std::vector<Jogador*> &ativos, std::vector<Jogador*> &vencedores );
        void aplicarPenalidadesLimiteEmpates( std::vector<Jogador*> &ativos );
        int contarJogadoresComSaldo() const;
        void declararCampeaoFinal();
        Fase esperarProximaFase( Fase ultimaFase );
        void mudarFase( Fase novaFase );
        void esperarTodosTerminarem( );
        void jogadorTerminouFase();
        bool isEncerrado();
        int getPote();
        void zerarPote();
        void aumentarPote( int valor );
        void encerrarJogo();

};

#endif