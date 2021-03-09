CMD cmdtbl[] = {
 { "g", (FP_LINE*) adj_g                  , ARG_LINE, " change ground level"},
 { "z", (FP_LINE*) adj_z                  , ARG_LINE, " change distance to ar_box"},
 { "f", (FP_LINE*) inc_var                , ARG_LINE, " increment variable"},
 { "n", (FP_LINE*) dec_var                , ARG_LINE, " decrement variable"},
 { "?", (FP_LINE*) help                   , ARG_LINE, " help message"},
 { "", (FP_LINE*)0                       , ARG_LINE, ""},
};
