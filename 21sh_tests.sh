    #!/bin/zsh

    tmpf="/tmp/toto"
    tests=(
    ## Execution ##
    "ls" # test execution simple
    "cat auteur" # execution simple
    "ls -l" # avec option
    "ls ~" # avec tilde
    "/bin/ls" # execution avec le chemin entier
    "/sbin/yubikey_shell" # execution sans les droits d’execution
    "yubikey_shell" # 
    "naritse" # cmd, élément qui n’existe pas
    "./21sh -c ls" # execution avec './'
    "./anrsiute" # execution élément qui n’existe pas
    "touch $tmpf ; chmod 000 $tmpf ; ./$tmpf" # execution toto sans les droits avec './'
    "$tmpf" # execution toto sans les droits avec le path entier
    "rm -f $tmpf"

    ## Builtin ##
    ## CD ##
    "cd ; pwd"
    "cd ~ ; pwd"
    "cd / ; pwd"
    "cd - ; pwd"
    "cd . ; pwd"
    "cd .. ; pwd"
    "cd ~/. ; pwd"
    "cd nasrt" # élément qui n’éxiste pas
    "cd auteur" # cd fichier

    ## ENV ##
    "env"
    "export TOTO=toto" # test setenv
    #"unsetenv TOTO"

    ## Other ##
    "pwd"
    "exit ; ls" # test exit: exit and don't display ls result

    ## Point-virgule
    "cat auteur ; cat auteur" # test un point-virgule
    "cat auteur ; cat auteur ; cat auteur" # test deux points-virgule

    ## Parsing ##
    ";;;;,;;;; ;;;;"
    "; ;"
    #"    \t  " # espaces, tabulations

    ## Pipe
    " " # empty
    "ls | cat" # pipe simple
    "ls | cat | cat" # pipe double
    "ls | cat | cat | cat" # pipe triple
    "ls -A -l | cat -e | cat -e" # pipe double with options
    "ls | cat | wc -l" # different commands
    "ls | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat \
    	| cat | cat | cat | cat | cat | cat | cat | cat | cat | cat" # twenty pipes
    "ls || cat" # Segfault
    "ls ||||||||| cat"
    "ls | | ls" # Segfault

    ## Redirections droite
    "ls > $tmpf ; cat $tmpf ; rm $tmpf" # simple
    "ls >> $tmpf ; cat $tmpf ; rm $tmpf" # double
    "mkdir 2> $tmpf; cat $tmpf ; rm $tmpf" # simple avec fd2 spécifié et erreur
    "mkdir 2>> $tmpf; cat $tmpf ; rm $tmpf" # double avec fd2 spécifié et erreur
    "ls 2> $tmpf; cat $tmpf ; rm $tmpf" # simple avec fd2 spécifié sans erreur
    "ls 2>> $tmpf; cat $tmpf ; rm $tmpf" # double avec fd2 spécifié sans erreur

    #"cat riri 2>&-" # avec fd spécifiés
    "ls>$tmpf ; cat $tmpf ; rm $tmpf" # collé
    "ls 1> $tmpf ; cat $tmpf" # fd 1 spécifié

    ## Redirections gauche
    "cat < $tmpf"
    #"cat << fin\nnrstai\nfin\n" # double gauche
    #"cat << toto\nnrstai\nrsnati\nairnst\ntoto" # double gauche avec autre string de fin

    ## Segfault on redirections
    ">"
    "ls>"
    "ls >"
    #"> toto", ">toto", ">> toto", ">>toto" # are not working
    ">>"
    "ls>>"
    "ls >>"
    "<"
    "cat<"
    "cat <"
    "< <<"
    "<toto"
    "<<"
    "cat<<"
    "cat <<"
    "<<end"
    "<< end"

    ## Mix
    "rm $tmpf ; ls ; pwd; ls -a | cat -e | wc > $tmpf ; cat $tmpf ; rm $tmpf" # point-virgules, pipes et redirection
    "ls -a | cat -e | wc >> $tmpf ; cat $tmpf ; rm $tmpf" # pipes et double redirection
    "mkdir test ; cd test ; ls -a ; ls | cat | wc -c > fifi ; cat fifi ; rm fifi ; cd .. ; rm -rf test"
    )

    TSH="21sh"
    SH="zsh"
    TSHO="/tmp/"$TSH"_output"
    SHO="/tmp/"$SH"_output"

    clear
    echo "\033[0;35m##### TESTS UNITAIRES DU SHELL #####\033[0m"

    for tu in $tests
    do
    	echo "\033[0;32m>>>> Test \"$tu\" <<<<\033[0m"
    	./$TSH -c $tu > $TSHO
    	$SH -c $tu > $SHO
    	diff $TSHO $SHO
    	echo ""
    done
    echo "\033[0;32m>>>> Test \"sh shell_script.sh\" <<<<\033[0m"
    ./$TSH shell_script.sh > $TSHO
    $SH shell_script.sh > $SHO
    diff $TSHO $SHO
