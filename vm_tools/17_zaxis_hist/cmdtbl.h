CMD cmdtbl[] = {
 { "0", (FP_LINE*) exp2mnual              , ARG_LINE, " manual exposure"},
 { "1", (FP_LINE*) exp2single             , ARG_LINE, " single shutter"},
 { "2", (FP_LINE*) exp2double             , ARG_LINE, " double shutter "},
 { "3", (FP_LINE*) exp2double_no_merge    , ARG_LINE, " double shutter for debug "},
 { "+", (FP_LINE*) incexp                 , ARG_LINE, " increment exposure value "},
 { "-", (FP_LINE*) decexp                 , ARG_LINE, " decrement exposure value "},
 { "s", (FP_LINE*) incnfil                , ARG_LINE, " stronger noise filter"},
 { "w", (FP_LINE*) decnfil                , ARG_LINE, " weaker noise filter"},
 { "?", (FP_LINE*) help                   , ARG_LINE, " help message"},
 { "", (FP_LINE*)0                       , ARG_LINE, ""},
};
