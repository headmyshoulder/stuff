; C++ modes 

(setq c-default-style '((c++-mode . "bsd")))
(setq c-basic-offset 4)
(setq-default indent-tabs-mode nil)

(add-to-list 'auto-mode-alist '("\\.cu$" . c++-mode))
(add-to-list 'auto-mode-alist '("\\.h\\'" . c++-mode))

(add-hook 'python-mode-hook
 (lambda ()
 (local-set-key (kbd "C-c C-c") 'comment-dwim)
 )
)



; ecb customization

(setq ecb-source-path '( ("/home/karsten/boost" "boost")
			 ("/home/karsten/src/test" "test")
			 ("/home/karsten/src/odeint-v2" "odeint")
			 ("/home/karsten/src/ambrosys/tc-supertoll/trunk/src" "SuperToll")
			 ("/home/karsten/src/ambrosys/tc-supertoll/util/AModTest" "AMod")
			 ("/home/karsten/src/ambrosys/tc-supertoll/util/TestSuite" "TestSuite")
			 ("/home/karsten/src/ambrosys/tc-supertoll/util/OsmTools" "OsmTools")
			 ("/home/karsten/src/ambrosys/tc-supertoll/util/ShapeFileTools" "ShapeFileTools")
			 ("/home/karsten/src/ambrosys/tc-supertoll/validierung" "Validierung")
			 ("/home/karsten/src/ambrosys/Amboss" "Amboss")
                         ("/home/karsten/src/ambrosys/tc-supertoll/data" "SuperToll Data")
                         ("/home/karsten/src/ambrosys/tc-supertoll/mv-app" "MV App")
                         ("/home/karsten/src/github_stuff" "Stuff")
                         ) )

(setq ecb-layout-name "left13")
(setq ecb-tip-of-the-day nil)


(global-set-key (kbd "<M-up>") 'ecb-goto-window-directories)
(global-set-key (kbd "<M-down>") 'ecb-goto-window-sources)
(global-set-key (kbd "<M-left>") 'ecb-goto-window-methods)
(global-set-key (kbd "<M-right>") 'ecb-goto-window-edit1)



; shortcuts
(global-set-key (kbd "C-c C-v") 'uncomment-region)


; tabbar customization
(setq tabbar-buffer-groups-function
      (lambda ()
	(list "All"))) ;; code by Peter Barabas

; yasnippet customization
; (add-to-list 'load-path "~/.emacs.d/plugins/yasnippet")
; (add-to-list 'load-path "/usr/share/emacs//site-lisp/yasnippet" )
; (require 'yasnippet)
; (yas/global-mode 1)


; startup modes
(ecb-activate)
(tabbar-mode)

; Tastenkombinationen:
;
; C-x o switch window
; o in buffer window open buffer
; C-x left next buffer
; C-x right previous buffer


(custom-set-variables  '(ecb-options-version "2.40")) 
(custom-set-faces )
