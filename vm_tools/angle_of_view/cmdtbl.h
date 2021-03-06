CMD cmdtbl[] = {
 { "0", (FP_LINE*) select_pt0             , ARG_LINE, " move point0"},
 { "1", (FP_LINE*) select_pt1             , ARG_LINE, " move point1"},
 { "d", (FP_LINE*) adj_dist               , ARG_LINE, " adjust distance"},
 { "y", (FP_LINE*) adj_vsize              , ARG_LINE, " adjust vertical size"},
 { "x", (FP_LINE*) adj_hsize              , ARG_LINE, " adjust horizontal size"},
 { "k", (FP_LINE*) mv_up                  , ARG_LINE, " move point up (like vim)"},
 { "j", (FP_LINE*) mv_down                , ARG_LINE, " move point down (like vim)"},
 { "h", (FP_LINE*) mv_left                , ARG_LINE, " move point left (like vim)"},
 { "l", (FP_LINE*) mv_right               , ARG_LINE, " move point right (like vim)"},
 { "+", (FP_LINE*) inc_var                , ARG_LINE, " increment dist./size"},
 { "-", (FP_LINE*) dec_var                , ARG_LINE, " decrement dist./size"},
 { "?", (FP_LINE*) help                   , ARG_LINE, " help message"},
 { "", (FP_LINE*)0                       , ARG_LINE, ""},
};
