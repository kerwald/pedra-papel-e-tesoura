#include <iostream>
#include <vector>
#include "Jogador.hpp"
#include "Jogo.hpp"

int main(){

    std::vector<Jogador> jogadores;
    jogadores.reserve( 6 );
    Jogo jogo( jogadores );
    
    std::cout << "Programa finalizado com sucesso." << std::endl;
    return 0;
}