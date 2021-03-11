CMD cmdtbl[] = {
 { "n", (FP_LINE*) set_near               , ARG_LINE, " set near side distance of z (cm)"},
 { "f", (FP_LINE*) set_far                , ARG_LINE, " set far side distance of z (cm)"},
 { "+", (FP_LINE*) inczdist               , ARG_LINE, " increment exposure value "},
 { "-", (FP_LINE*) deczdist               , ARG_LINE, " decrement exposure value "},
 { "?", (FP_LINE*) help                   , ARG_LINE, " help message"},
 { "", (FP_LINE*)0                       , ARG_LINE, ""},
};
