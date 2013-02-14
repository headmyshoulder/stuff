; C++ modes 

(add-to-list 'load-path "~/.emacs.d/ecb")
(load-file "~/.emacs.d/cedet/common/cedet.el")

(require 'ecb-autoloads)



(setq c-default-style '((c++-mode . "bsd")))
(setq c-basic-offset 4)
(c-set-offset 'innamespace 0)
(setq-default indent-tabs-mode nil)

(add-to-list 'auto-mode-alist '("\\.cu$" . c++-mode))
(add-to-list 'auto-mode-alist '("\\.h\\'" . c++-mode))
(add-to-list 'auto-mode-alist '("\\.xsd\\'" . xml-mode))
(add-to-list 'auto-mode-alist '("\\.xslt\\'" . xml-mode))

(add-hook 'python-mode-hook
 (lambda ()
 (local-set-key (kbd "C-c C-c") 'comment-dwim)
 )
)

; ediff customization
(setq ediff-split-window-function 'split-window-horizontally)


; cedet customization
(global-ede-mode t)

(semantic-load-enable-minimum-features)
(global-semantic-tag-folding-mode)

(require 'semantic-ia)
(require 'semantic-gcc)
(semantic-add-system-include "~/boost/boost_1_50_0" 'c++-mode)

(defun my-cedet-hook ()
  (local-set-key [(control return)] 'semantic-ia-complete-symbol)
  (local-set-key "\C-c?" 'semantic-ia-complete-symbol-menu)
  (local-set-key "\C-c>" 'semantic-complete-analyze-inline)
  (local-set-key "\C-cp" 'semantic-analyze-proto-impl-toggle))

(add-hook 'c-mode-common-hook 'my-cedet-hook)


; ecb customization

(setq ecb-source-path '( ( "/home/karsten/boost" "boost" )
			 ( "/home/karsten/src/test" "test" )
			 ( "/home/karsten/src/odeint-v2" "odeint" )
                         ( "/home/karsten/src/ambrosys" "ambrosys" )
                         ( "/home/karsten/src/ambrosys/tc-supertoll/trunk/src" "SuperToll" )
                         ( "/home/karsten/src/ambrosys/Amboss" "Amboss" )
                         ( "/home/karsten/src/ambrosys/tc-supertoll/data" "SuperTollData" )
                         ( "/home/karsten/src/genetic_programming" "GP" )
                         ( "/home/karsten/src/ambrosys/tc-supertoll/branches/bd_new" "SuperTollRedesign" )
                       ))

; (setq ecb-layout-name "left13")
; (setq ecb-layout-name "left3")
(setq ecb-layout-name "leftright3")
(setq ecb-show-sources-in-directories-buffer '("left7" "left13" "left14" "left15" "leftright3" "left3"))
(setq ecb-tip-of-the-day nil)


(global-set-key (kbd "<M-up>") 'ecb-goto-window-directories)
(global-set-key (kbd "<M-down>") 'ecb-goto-window-sources)
(global-set-key (kbd "<M-left>") 'ecb-goto-window-methods)
(global-set-key (kbd "<M-right>") 'ecb-goto-window-edit1)
(global-set-key (kbd "M-.") 'semantic-ia-fast-jump)


; shortcuts
(global-set-key (kbd "C-c C-v") 'uncomment-region)


; tabbar customization
(setq tabbar-buffer-groups-function
      (lambda ()
	(list "All"))) ;; code by Peter Barabas





; startup modes
(ecb-activate)          ; ecb
(tabbar-mode)           ; tabbar
(global-subword-mode 1) ; camel case


; supress scratch buffer
(kill-buffer "*scratch*")
(setq message-log-max nil)

; copy and pasting
(setq x-select-enable-clipboard t) ; as above
(setq interprogram-paste-function 'x-cut-buffer-or-selection-value)



(ede-cpp-root-project "SuperToll"
                :name "SuperToll"
                :file "~/src/ambrosys/tc-supertoll/trunk/src/Jamroot"
                :include-path '("/SuperToll"  )
                :system-include-path '("~/boost/boost_1_51_0/")
                :spp-table '(("isUnix" . "")
                             ("BOOST_TEST_DYN_LINK" . "")))

(ede-cpp-root-project "odeint"
                :name "odeint"
                :file "~/src/odeint-v2/Jamroot"
                :include-path '("/"  )
                :system-include-path '("~/boost/boost_1_51_0/")
                :spp-table '(("isUnix" . "")
                             ("BOOST_TEST_DYN_LINK" . "")))

(ede-cpp-root-project "GP"
                :name "GP"
                :file "~/src/genetic_programming/CMakeLists.txt"
                :include-path '("/"  )
                :system-include-path '("~/boost/boost_1_51_0/")
                :spp-table '(("isUnix" . "")
                             ("BOOST_TEST_DYN_LINK" . "")))


(ede-cpp-root-project "SuperTollRedesign"
                :name "SuperTollRedesign"
                :file "~/src/ambrosys/tc-supertoll/branches/bd_new/Jamroot"
                :include-path '("/SuperToll"  )
                :system-include-path '("~/boost/boost_1_51_0/")
                :spp-table '(("isUnix" . "")
                             ("BOOST_TEST_DYN_LINK" . "")))



; ???
(custom-set-variables '(ecb-options-version "2.40") )

(custom-set-faces )





; Tastenkombinationen:
;
; C-x o switch window
; o in buffer window open buffer
; C-x left next buffer
; C-x right previous buffer


