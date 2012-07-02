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

(setq ecb-source-path '(
			"/home/karsten/boost"
                        "/home/karsten/src/"
			"/home/karsten/src/test"
			"/home/karsten/src/odeint-v2"
                        "/home/karsten/src/ambrosys"
                       ))
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
