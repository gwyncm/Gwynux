:if has("terminfo")
   :  set t_Co=8
   :  set t_Sf=[3%p1%dm
   :  set t_Sb=[4%p1%dm
   :else
   :  set t_Co=8
   :  set t_Sf=[3%dm
   :  set t_Sb=[4%dm
   :endif
:syntax on
