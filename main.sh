#!/bin/bash
set -e

# Garante que o executável antigo seja removido antes de compilar
rm -f main


# Imprime a arte ASCII de forma segura
cat << 'EOF'

                                                                                                                      
                                                            ____                ___      ,---,                        
                                                          ,'  , `.            ,--.'|_  ,--.' |                        
             ,---.           ,--,      ,-.             ,-+-,.' _ |   ,---.    |  | :,' |  |  :                __  ,-. 
            '   ,'\        ,'_ /|,' ,'/ /|          ,-+-. ;   , ||  '   ,'\   :  : ' : :  :  :              ,' ,'/ /| 
      .--, /   /   |  .--. |  | :'  | |' |         ,--.'|'   |  || /   /   |.;  ,'  /  :  |  |,--.   ,---.  '  | |' | 
    /_ ./|.   ; ,. :,'_ /| :  . ||  |   ,'        |   |  ,', |  |,.   ; ,. :|  |   |   |  :  '   |  /     \ |  |   ,' 
 , ' , ' :'   | |: :|  ' | |  . .'  :  /          |   | /  | |--' '   | |: ::  ,'| :   |  |   /' : /    /  |'  :  /   
/___/ \: |'   | .; :|  | ' |  | ||  | '           |   : |  | ,    '   | .; :  '  : |   '  :  | | |.    ' / ||  | '    
 .  \  ' ||   :    |:  | : ;  ; |;  : |           |   : |  |/     |   :    |  |  | '.'||  |  ' | :'   ;   /|;  : |    
  \  ;   : \   \  / '  :  `--'   \  , ;           |   | |`-'       \   \  /   ;  :    ;|  :  :_:,''   |  / ||  , ;    
   \  \  ;  `----'  :  ,      .-./---'            |   ;/            `----'    |  ,   / |  | ,'    |   :    | ---'     
    :  \  \          `--`----'                    '---'                        ---`-'  `--''       \   \  /           
     \  ' ;                                                                                         `----'            
      `--`
EOF

gcc -std=c99 -Wall -o main BANCO_ACME.c

# Se não houver argumentos, executa os casos de teste
if [ $# -eq 0 ]; then
    ./main 0
else
    ./main "$@"
fi