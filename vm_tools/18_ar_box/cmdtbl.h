CMD cmdtbl[] = {
 { "f", (FP_LINE*) inc_z                  , ARG_LINE, " increment distance to box (z axis) "},
 { "n", (FP_LINE*) dec_z                  , ARG_LINE, " decrement distance to box (z axis) "},
 { "?", (FP_LINE*) help                   , ARG_LINE, " help message"},
 { "", (FP_LINE*)0                       , ARG_LINE, ""},
};
