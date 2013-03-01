# ~/.bashrc: executed by bash(1) for non-login shells.
# see /usr/share/doc/bash/examples/startup-files (in the package bash-doc)
# for examples

# If not running interactively, don't do anything
[ -z "$PS1" ] && return

# don't put duplicate lines in the history. See bash(1) for more options
export HISTCONTROL=ignoredups

# check the window size after each command and, if necessary,
# update the values of LINES and COLUMNS.
shopt -s checkwinsize

# make less more friendly for non-text input files, see lesspipe(1)
[ -x /usr/bin/lesspipe ] && eval "$(lesspipe)"

# set variable identifying the chroot you work in (used in the prompt below)
if [ -z "$debian_chroot" ] && [ -r /etc/debian_chroot ]; then
    debian_chroot=$(cat /etc/debian_chroot)
fi

# set a fancy prompt (non-color, unless we know we "want" color)
case "$TERM" in
xterm-color)
    PS1='${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '
    ;;
*)
    PS1='${debian_chroot:+($debian_chroot)}\u@\h:\w\$ '
    ;;
esac

# Comment in the above and uncomment this below for a color prompt
#PS1='${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '

# If this is an xterm set the title to user@host:dir
case "$TERM" in
xterm*|rxvt*)
    PROMPT_COMMAND='echo -ne "\033]0;${USER}@${HOSTNAME}: ${PWD/$HOME/~}\007"'
    ;;
*)
    ;;
esac

# Alias definitions.
# You may want to put all your additions into a separate file like
# ~/.bash_aliases, instead of adding them here directly.
# See /usr/share/doc/bash-doc/examples in the bash-doc package.

#if [ -f ~/.bash_aliases ]; then
#    . ~/.bash_aliases
#fi

# enable color support of ls and also add handy aliases
if [ "$TERM" != "dumb" ]; then
    eval "`dircolors -b`"
    alias ls='ls --color=auto'
    #alias dir='ls --color=auto --format=vertical'
    #alias vdir='ls --color=auto --format=long'
fi

# some more ls aliases
#alias ll='ls -l'
#alias la='ls -A'
alias l='ls -la --color=auto'
alias ls='ls --color=auto'
alias grep='grep --color=auto'

alias md='mkdir'

# enable programmable completion features (you don't need to enable
# this, if it's already enabled in /etc/bash.bashrc and /etc/profile
# sources /etc/bash.bashrc).
if [ -f /etc/bash_completion ]; then
    . /etc/bash_completion
fi

PATH=$PATH:./:/home/karsten/bin:/opt/matlab/bin
# export PATH=$PATH:./:$HOME/bin

# JAVA_HOME=/usr/lib64/jvm/java-1.6.0-openjdk-1.6.0
# export PATH=$PATH:$JAVA_HOME/bin

# export JBOSS_HOME=$HOME/libs/jboss
# export PATH=$PATH:$JBOSS_HOME/bin

# export ANT_HOME=$HOME/libs/ant
# export PATH=$PATH:$ANT_HOME/bin


# export PS1='\[\033[0;32m\]\h \[\033[01;34m\]\W \$ \[\033[00m\]'

# Show git branch in bash prompt
function git_branch {
    echo "$(git branch 2>/dev/null | sed -e '/^*/!d' -e 's/^* \(.*\)/ (\1)/')"
}

# modified
export PS1="\[\e[01;32m\]\h \[\e[01;34m\]\W\
\`if [ -n \"\$(git status -s -uno 2>/dev/null)\" ]; then
echo -n \[\e[31m\]; else echo -n \[\e[34m\]; fi;
git_branch\`\
 $\[\e[0m\] "

# Original version from Denis
#export PS1="\[\e[32m\][\u@\[\e[34m\]\h \[\e[33m\]\W\
#\`if [ -n \"\$(git status -s 2>/dev/null)\" ]; then
#echo -n \[\e[31m\]; else echo -n \[\e[34m\]; fi;
#git_branch\`\
#\[\e[32m\]]$\[\e[0m\] "


export EDITOR=/usr/bin/nano

export BOOST_ROOT=$HOME/boost/boost_1_53_0
export BOOST_BUILD_PATH=$BOOST_ROOT/tools/build/v2
export BOOST_LIBRARYDIR=$BOOST_ROOT/state/lib

# export BLITZ_ROOT=$HOME/boost/blitz-0.9
# export MTL4_ROOT=$HOME/boost/mtl4
export ODEINT_ROOT=$HOME/src/odeint-v2
# export THRUST_ROOT=$HOME/boost/thrust
# export CUDA_ROOT=/usr/local/cuda/include
# export LAZYTYPES_ROOT=$HOME/boost/additional/lazytypes
# export MISC_HEADERS=$HOME/boost/misc_headers
# export CARBON_ROOT=$THRUST_ROOT/carbon
# export THRUST_EXT_ROOT=$HOME/src/mpc/mpc/thrust_ext
export AMBOSS_ROOT=$HOME/src/ambrosys/Amboss
export VEXCL_ROOT=$HOME/boost/vexcl
# export VIENNACL_ROOT=$HOME/boost/testing/ViennaCL-1.3.0
# export VIENNACL_ROOT=$HOME/boost/testing/ViennaCL-1.4.0-preview
export VIENNACL_ROOT=$HOME/boost/testing/viennacl
export EIGEN_ROOT=$HOME/boost/testing/eigen

export OSMIUM_ROOT=$HOME/boost/testing/osmium
export OSMPBF_ROOT=$HOME/boost/testing/osmpbf
export GDAL_ROOT=$HOME/boost/gdal-1.9.2/install ;

# export LD_LIBRARY_PATH=/home/karsten/boost/lib


if [ -f /opt/intel/composerxe/mkl/bin/mklvars.sh ]
then
source /opt/intel/composerxe/mkl/bin/mklvars.sh intel64
fi

export PYTHONPATH=$HOME/src/ambrosys/tc-supertoll/util/TestSuite:$HOME/src/ambrosys/tc-supertoll/util/Validierung
export PATH=$PATH:$HOME/bin



if [ -f /usr/share/GNUstep/Makefiles/GNUstep.sh ]
then
. /usr/share/GNUstep/Makefiles/GNUstep.sh 
fi

export CLASSPATH=$CLASSPATH:$HOME/boost/testing/ecj/ec
