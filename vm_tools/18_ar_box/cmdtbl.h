CMD cmdtbl[] = {
 { "g", (FP_LINE*) adj_g                  , ARG_LINE, " change ground level"},
 { "h", (FP_LINE*) adj_h                  , ARG_LINE, " change altitude of platform"},
 { "i", (FP_LINE*) adj_m                  , ARG_LINE, " display mode "},
 { "+", (FP_LINE*) inc_var                , ARG_LINE, " increment variable"},
 { "-", (FP_LINE*) dec_var                , ARG_LINE, " decrement variable"},
 { "?", (FP_LINE*) help                   , ARG_LINE, " help message"},
 { "", (FP_LINE*)0                       , ARG_LINE, ""},
};
