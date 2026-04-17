#include <iostream>
#include <vector>
#include "Jogador.hpp"
#include "Jogo.hpp"

int main(){

    std::vector<Jogador> *jogadores = new std::vector<Jogador>;
    Jogo *jogo = new Jogo( *jogadores );
    
}