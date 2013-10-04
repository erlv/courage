(add-to-list 'load-path "~/.emacs.d/org-8.0.3/lisp/")
(add-to-list 'load-path "~/.emacs.d/")
(add-to-list 'load-path "/usr/share/emacs/site-lisp/")
(require 'color-theme)
(require 'sr-speedbar)
(color-theme-initialize)
(color-theme-aalto-light)
;(global-visual-line-mode t)

;enable auctexparse
;; (load "auctex.el" nil t t)
;; (load "preview-latex.el" nil t t)
;; (setq TeX-auto-save t)
;; (setq TeX-parse-self t)
;; (setq-default TeX-master nil)
;; (add-hook 'LaTeX-mode-hook 'LaTeX-install-toolbar)
;; (add-hook 'LaTeX-mode-hook 'turn-on-auto-fill)
;; (add-hook 'LaTeX-mode-hook 'visual-line-mode)
;; (add-hook 'LaTeX-mode-hook 'flyspell-mode)
;; (add-hook 'LaTeX-mode-hook 'LaTeX-math-mode)
;; (add-hook 'LaTeX-mode-hook 'turn-on-reftex)
;; (setq reftex-plug-into-AUCTeX t)

;; (add-hook 'LaTeX-mode-hook (lambda()
;;                              (add-to-list 'TeX-command-list
;; 					  '("XeLatex" "%`xelatex%(mode)%' %t" TeX-run-TeX nil t))
;;                              (setq TeX-command-default "XeLatex")
;;                              (setq TeX-save-query nil )
;;                              (setq TeX-show-compilation t)
;;                              ))

;; (setq TeX-output-view-style
;;       (quote
;;        (("^pdf$" "." "evince -f %o")
;;         ("^html?$" "." "firefox %o"))))

;; display line num
(setq column-number-mode t)
(setq line-number-mode t)
(global-linum-mode 1)

;; display the time
(setq display-time-24hr-format t)
(setq display-time-day-and-date t)
(display-time)

;; splash message off
(setq inhibit-startup-message t)

;; open image display
(auto-image-file-mode t)

;; stop display the toolbar in the head
(tool-bar-mode 0)

;; delete the scroll bar
(scroll-bar-mode 0)

;; auto move the mouse pointer when it cover the display
;;(mouse-avoidance-mode 'animate)

;; support middle button paste
(setq mouse-yank-at-point t)

;; support paste from x clipboard
(setq x-select-enable-clipboard t)


;; new buffer menu look
(require 'ibuffer)
(global-set-key (kbd "C-x C-b") 'ibuffer)

;;ido mode to select buffers
(require 'ido)
(ido-mode t)
(put 'upcase-region 'disabled nil)

(put 'downcase-region 'disabled nil)

;; show more file  path information in status bar
(require 'uniquify)
(setq uniqueify-buffer-name-style 'reverse)

;; set F5 to refresh the buffer
(global-set-key (kbd "<f5>") 'revert-buffer)


;; Set emacs shell to be /bin/bash
(setq shell-file-name "/bin/bash")
(autoload 'ansi-color-for-comint-mode-on "ansi-color" nil t)
(add-hook 'shell-mode-hook 'ansi-color-for-comint-mode-on t)


(setq gnus-button-url 'browse-url-generic
      browse-url-generic-program "firefox"
      browse-url-browser-function gnus-button-url)

(add-to-list 'auto-mode-alist '("\\.markdown" . markdown-mode))
(add-to-list 'auto-mode-alist '("\\.md" . markdown-mode))
(add-to-list 'auto-mode-alist '("\\.text\\'" . markdown-mode))

(custom-set-variables
  ;; custom-set-variables was added by Custom.
  ;; If you edit it by hand, you could mess it up, so be careful.
  ;; Your init file should contain only one such instance.
  ;; If there is more than one, they won't work right.
 '(column-number-mode t)
 '(display-time-mode t)
 '(tool-bar-mode nil))
;;(custom-set-faces
  ;; custom-set-faces was added by Custom.
  ;; If you edit it by hand, you could mess it up, so be careful.
  ;; Your init file should contain only one such instance.
  ;; If there is more than one, they won't work right.
;; '(default ((t (:inherit nil :stipple nil :background "dark slate gray" :foreground "blanched almond" :inverse-video nil :box nil :strike-through nil :overline nil :underline nil :slant normal :weight normal :height 113 :width normal :foundry "unknown" :family "文泉驿等宽微米黑")))))

(require 'recentf)
(recentf-mode 1)
(global-set-key "\C-xf" 'recentf-open-files)
(setq recentf-auto-cleanup 'never)
(custom-set-faces
  ;; custom-set-faces was added by Custom.
  ;; If you edit it by hand, you could mess it up, so be careful.
  ;; Your init file should contain only one such instance.
  ;; If there is more than one, they won't work right.
 '(default ((t (:inherit nil :stipple nil :background "gray12" :foreground "green" :inverse-video nil :box nil :strike-through nil :overline nil :underline nil :slant normal :weight normal :height 113 :width normal :foundry "unknown" :family "文泉驿等宽微米黑")))))


;; Always highlight FIXME, TODO and BUG keyword
(add-hook 'c-mode-common-hook
	  (lambda()
	    (font-lock-add-keywords nil
				    '(("\\<\\(FIXME\\|TODO\\|BUG\\):" 1 font-lock-warning-face t)))))
;; Markdown highlight keyword
(add-hook 'markdown-mode-hook
	  (lambda()
	    (font-lock-add-keywords nil
				    '(("\\<\\(FIXME\\|TODO\\|BUG\\):" 1 font-lock-warning-face t)))))

(setq default-buffer-file-coding-system 'utf-8)
(prefer-coding-system 'utf-8)
(put 'scroll-left 'disabled nil)

;; the in-frame speedbar
(when (require 'sr-speedbar nil 'noerror)
  (setq speedbar-supported-extension-expressions
    '(".org" ".[ch]\\(\\+\\+\\|pp\\|c\\|h\\|xx\\)?"
       ".tex\\(i\\(nfo\\)?\\)?" ".el"
       ".java" ".p[lm]" ".pm" ".py"  ".s?html"  "Makefile.am" "configure.ac"))
  (setq
    sr-speedbar-width-x 20
    sr-speedbar-right-side t))
(when (fboundp 'sr-speedbar-toggle)
  (global-set-key (kbd "C-<f9>") 'sr-speedbar-toggle)
  (global-set-key (kbd "C-<f10>") 'sr-speedbar-select-window)) ; speedbar
(put 'narrow-to-region 'disabled nil)

;; For auto complete config
(require 'auto-complete-config)
(add-to-list 'ac-dictionary-directories "~/.emacs.d/ac-dict")
(ac-config-default)
(setq ac-delay 0.4) ;; eclipse uses 500ms
(define-key ac-completing-map "\ESC/" 'ac-stop)

;; Show paren
(show-paren-mode 1)
(require 'highlight-parentheses)
(highlight-parentheses-mode 1)

;; Buffers with the same filename
(require 'uniquify)
(setq uniquify-buffer-name-style 'post-forward
      uniquify-separator ":")


;; Code HS mode
(add-hook 'c-mode-hook 'hs-minor-mode)
(add-hook 'c++-mode-hook 'hs-minor-mode)  