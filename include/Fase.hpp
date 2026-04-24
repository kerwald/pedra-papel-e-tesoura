#pragma once
#include <cstdint> 

enum class Fase {
    ESPERA,
    APOSTA,
    AUMENTARAPOSTA,
    JOGADA,
    ENCERRADO
};

inline std::ostream& operator<<( std::ostream& os, Fase fase ) {
    switch ( fase ) {
        case Fase::ESPERA:    os << "ESPERA";    break;
        case Fase::APOSTA:    os << "APOSTA";    break;
        case Fase::AUMENTARAPOSTA:    os << "AUMENTAR APOSTA";    break;
        case Fase::JOGADA:    os << "JOGADA";    break;
        case Fase::ENCERRADO: os << "ENCERRADO"; break;
    }
    return os;
}