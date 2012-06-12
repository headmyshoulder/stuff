(setq ecb-source-path '(
			"/home/karsten/src/test"
			"/home/karsten/boost"
			"/home/karsten/src/odeint-v2"
                        "/home/karsten/src/taylor"
                       ))

; (setq c-default-style "cc-mode")

(setq c-default-style "bsd"
      c-basic-offset 4)




(global-set-key (kbd "<M-up>") 'ecb-goto-window-directories)
(global-set-key (kbd "<M-down>") 'ecb-goto-window-sources)
(global-set-key (kbd "<M-left>") 'ecb-goto-window-methods)
(global-set-key (kbd "<M-right>") 'ecb-goto-window-edit1)

(global-set-key (kbd "C-c C-v") 'uncomment-region)
(global-set-key (kbd "<Ret>") 'switch-to-buffer-other-window)

(setq ecb-layout-name "left13")
(setq ecb-tip-of-the-day nil)

(add-to-list 'auto-mode-alist '("\\.cu$" . c++-mode))

(add-hook 'python-mode-hook
 (lambda ()
 (local-set-key (kbd "C-c C-c") 'comment-dwim)
 )
)

; Tastenkombinationen:
;
; C-x o switch window
; o in buffer window open buffer
; C-x left next buffer
; C-x right previous buffer
