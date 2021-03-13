CMD cmdtbl[] = {
 { "m", (FP_LINE*) chg_mode               , ARG_LINE, " change display image"},
 { "g", (FP_LINE*) adj_g                  , ARG_LINE, " change ground level"},
 { "h", (FP_LINE*) adj_h                  , ARG_LINE, " change altitude of platform"},
 { "b", (FP_LINE*) adj_b                  , ARG_LINE, " change arbox "},
 { "i", (FP_LINE*) adj_m                  , ARG_LINE, " impose depth or not "},
 { "r", (FP_LINE*) adj_r                  , ARG_LINE, " set rolling angle in degree"},
 { "p", (FP_LINE*) adj_p                  , ARG_LINE, " set pitching angle in degree"},
 { "+", (FP_LINE*) inc_var                , ARG_LINE, " increment variable"},
 { "-", (FP_LINE*) dec_var                , ARG_LINE, " decrement variable"},
 { "?", (FP_LINE*) help                   , ARG_LINE, " help message"},
 { "", (FP_LINE*)0                       , ARG_LINE, ""},
};
