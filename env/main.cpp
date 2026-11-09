#include "renderer.h"
#include "evaluator.h"
#include "strategies/random.h"
#include "strategies/lean.h"

int main() {
    RandomStrategy s;
    Observer *o = new Observer();
    Evaluator e(1000, 42, o);
    // printReport(e.evaluate(s));

    LeanStrategy l;
    printReport(e.evaluate(l));

    return 0;
}