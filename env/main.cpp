#include "renderer.h"
#include "evaluator.h"
#include "strategies/random_strategy.h"

int main() {
    RandomStrategy s;
    Evaluator e(1000, 42);
    printReport(e.evaluate(s));

    watch(s, 42, 10);
    return 0;
}