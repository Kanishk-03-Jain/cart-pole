#include "renderer.h"
#include "evaluator.h"
#include "strategies/random_strategy.h"

int main() {
    RandomStrategy s;
    Observer o;
    Evaluator e(1000, 42, &o);
    printReport(e.evaluate(s));

    return 0;
}