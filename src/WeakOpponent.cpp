#include "WeakOpponent.h"

std::pair<int,int> WeakOpponent::chooseMove(const Board& b, int me) {
    int opp = -me;

    auto tryCompleteLine = [&](int player)->std::pair<int,int>{
        for (auto [r,c] : b.legalMoves()){
            Board nb = b;
            nb.place(r,c,player);
            if (nb.winner() == player) return {r,c};
        }
        return {-1,-1};
    };

    auto mv = tryCompleteLine(me);
    if (mv.first != -1) return mv;

    mv = tryCompleteLine(opp);
    if (mv.first != -1) return mv;

    static const std::pair<int,int> pref[] = {
        {0,0},{0,1},{0,2},{1,0},{1,2},{2,0},{2,1},{2,2},{1,1}
    };
    for (auto [r,c] : pref) if (b.isEmpty(r,c)) return {r,c};

    auto moves = b.legalMoves();
    return moves.empty() ? std::pair<int,int>{-1,-1} : moves[0];
}
