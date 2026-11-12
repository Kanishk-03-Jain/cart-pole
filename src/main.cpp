#include "renderer/renderer.h"
#include "evaluation/evaluator.h"
#include "strategies/random.h"
#include "strategies/lean.h"

int main() {
    // RandomStrategy s;
    EpisodeObserver *o = new WindowObserver();
    Evaluator e(1000, 42, o);
    // printReport(e.evaluate(s));

    LeanStrategy l;
    printReport(e.evaluate(l));
    delete o;
    return 0;
}