;; Get rid of gre-system-version?

;;; gre-mode.el - major mode for Gre files

;; Copyright (C) 1997-1998 Peter S. Galbraith
 
;; Author:    Peter S. Galbraith <galbraith@mixing.Qc.DFO.ca>
;; Created:   11 Feb 1997
;; Version:   1.07 (04 May 2000)
;; Keywords:  gre, Emacs, XEmacs, graphics.

;; RCS $Id: gre-mode.el,v 1.7 2000/06/29 18:20:03 kelley Exp $
;; Note: RCS version number does not correspond to release number.

;; Everyone is granted permission to copy, modify and redistribute this
;; file provided:
;;   1. All copies contain this copyright notice.
;;   2. All modified copies shall carry a prominant notice stating who
;;      made modifications and the date of such modifications.
;;   3. The name of the modified file be changed.
;;   4. No charge is made for this software or works derived from it.
;;      This clause shall not be construed as constraining other software
;;      distributed on the same medium as this software, nor is a
;;      distribution fee considered a charge.

;; ----------------------------------------------------------------------------
;;; Commentary:

;; This major mode for GNU Emacs (and XEmacs) provides support for editing
;; Gre files.  Gre is a graphics plotting package (language) that produces
;; beautiful postscript output suitable for publications.  
;; Gre is written by Dan Kelley, Dalhousie University.  
;; Info is available at http://www.phys.ocean.dal.ca/~kelley/gre/

;; New versions of gre-mode may be obtained at:
;;   ftp://ftp.phys.ocean.dal.ca/users/rhogee/elisp/gre-mode.el

;;; Features of gre-mode include:
;;
;;   Automatic indentation for block structures, line continuations, 
;;     and comments.  
;;   Gre command and variable completion using Meta-Tab [M-tab].
;;   Displays help or info-manual about the gre command on the current line
;;    or for a command prompted-for with Tab completion.  
;;   Hide user commands at the beginning of gre buffers (using hideshow's
;;     hs-minor-mode)
;;   imenu support to quickly move to command or variable definition.
;;   Run gre from within emacs.
;;   Display (possibly compressed) postscript file associated with gre file.
;;   Font-lock support to colourise your gre code as you type it in.
 
;;; Using gre-mode in a nutshell:
;;
;;  - Edit a gre file to enter gre-mode.
;;  - Available commands are available from pull-down menus, and keystroke
;;    short-cuts are indicated.
;;  - Press `C-h m' to get help on the mode (a short description of commands).
;;  - Use `M-Tab' to complete commands you are typing in.
;;  - Use `C-c C-k' to kill (delete) a displayed option in the full syntax line
;;        `C-c C-o' to select an option within square brackets.
;;  - Use the help and info commands to get syntax info on all gre commands.
;;  - Use `C-c C-r' to run gre on your buffer, and display the postscript.
;;  - Use `C-c C-v' to view the (compressed?) postscript file you generate.
;;  - Use `gre-set-version' and `gre-set-local-version' commands to use 
;;    different installed versions of gre.

;;; Installation  -- Follow these 3 (or 4) steps:
;;
;; 0- Configuring Emacs to load your extra .el files
;;
;;  Extra .el files like gre-mode.el that are not part of Emacs should be
;;  stored in a directory where Emacs will find them when you ask it to
;;  load them.  The files should therefore be found in Emacs' `load-path'.
;;  To see the directory list currently in the load-path, do
;;
;;    C-h v load-path
;;
;;  If you have access to system directories, put gre-mode.el in a
;;  `site-lisp' directory, such as `/usr/local/share/emacs/site-lisp/'
;;  That way all users will have access to the files.
;;
;;  If you don't have access to a site-lisp directory (e.g. you have only a
;;  user account), then create a directory where your extra .el files will
;;  be stored and add it to Emacs' load-path.  For example, say you created
;;  /home/psg/emacs and stored gre-mode.el there, you would then put this
;;  near the top of your ~/.emacs file:
;;
;;    (setq load-path (cons "/home/psg/emacs" load-path))
;;
;; 1 - Configuring gre-mode to where gre lives on your system:
;;
;;  This step may be omitted if you never wish to run different versions of
;;  gre, or if only a single version is ever installed at one time on your
;;  system.
;;
;;  Gre can support multiple versions of installed gre (See commands 
;;  `gre-set-version' and `gre-set-local-version').  But to use this 
;;  feature you must set the variable `gre*directory-tree'.
;;  -
;;  If you use an opt-style gre directory tree, set gre*directory-tree to
;;  the root of this tree.  For example, if your gre files reside in
;;  directories like:
;;
;;    /home/opt/gre/2.0.17/bin/gre
;;    /home/opt/gre/2.0.17/lib/gre.cmd
;;    /home/opt/gre/2.0.18/bin/gre
;;    /home/opt/gre/2.0.18/lib/gre.cmd
;;
;;  then you'd use:
;;    (setq gre*directory-tree "/home/opt/gre/")
;;  -
;;  If you use a Red Hat or Debian installation like:
;;
;;    /usr/bin/gre -> /usr/bin/gre-2.1.17
;;    /usr/lib/gre/2.1.17/gre.cmd
;;  or, optionnaly with the `lib' directory below the VERSION directory:
;;    /usr/bin/gre -> /usr/bin/gre-2.1.17
;;    /usr/lib/gre/2.1.17/lib/gre.cmd
;;
;;  then you'd use:
;;    (setq gre*directory-tree "/usr/lib/gre/")
;;
;;  Note that since there is no /usr/lib/gre/2.1.17/bin/ directory in this
;;  example, gre binaries with version number suffixes must exist in
;;  the path (e.g the shell command `gre-2.1.17' must exist.)
;;  -
;;  You may have more than one tree and make a list of them:
;;
;;    (setq gre*directory-tree '("/home/opt/gre/" "/usr/lib/gre/"))
;;
;; 2 - Telling emacs to load gre-mode:
;;
;;  To tell emacs to use this mode when editing .gre files, you can load
;;  gre-mode whenever a new emacs session is starting by adding the
;;  following line to your ~/.emacs file:
;;
;;   (require 'gre-mode)
;;
;;  This is a good thing specially when you only start emacs once a week and
;;  use it for every file you edit (as you should).  
;;  If you startup a fresh emacs every time you edit (bad scientist!)
;;  then you probably only want to load gre-mode into emacs when you need it.
;;  In that case, instead of the `require' statement above, add the following 
;;  lines to your ~/.emacs file:
;;
;;   (autoload 'gre-mode "gre-mode" "Enter Gre-mode." t)
;;   (setq auto-mode-alist (cons '("\\.gre$" . gre-mode) auto-mode-alist))
;;
;; 3 - Extra user configuration of gre-mode
;;
;;  At this point, gre-mode should start up when you edit a gre file.  You
;;  may optionally customize gre-mode by setting variables in your ~/.emacs
;;  file.  These are briefly described by typing `C-h m' while in gre-mode,
;;  and are listed in this file before the line identified with `End of
;;  user code.'  Then, for further infomation, use emacs' describe-variable
;;  command, bound to `C-h v'.  For example, for more information about the
;;  gre*WWW-program variable, you'd type `C-h v gre*WWW-program' (note that
;;  emacs does [TAB] completion, so pressing the [TAB] key after typing-in
;;  `gre*' will display all gre related variables.)

;; ----------------------------------------------------------------------------
;;; Change log:
;;
;; V1.07 04May2000 Dan Kelley <Dan.Kelley@Dal.CA> (RCS V1.17)
;;  - tweaked font-lock pattern for variables (no minus sign).
;; V1.06 18Apr2000 PSG (RCS V1.16)
;;  - Made gre*directory-tree default to new /usr/local/share/gre location
;; V1.05 18Apr2000 PSG (RCS V1.15)
;;  - added gre*view-scale along with menubar selection
;; V1.04 16Dec99 PSG
;;  - fixed gre-info-function for xemacs20
;;  - Default gre*view-command is now gv.
;; V1.03 16Jun99 PSG (RCS V1.14)
;;  - tweak beacuse xemacs doesn't have imenu (it must have an equivalent?)
;; V1.03 03Sep98 PSG (RCS V1.13)
;;  - changed gre-run to better diagnose gre-version availability.
;; V1.02 21Aug98 PSG (RCS V1.11)
;;  - changed gre*WWW-manual 
;; V1.01 27Jul98 PSG (RCS V1.8)
;;  - Changed gre*directory-tree to allow list or be completely unset.
;; V1.00 23apr97 Peter Galbraith - Created from gri-mode.el 

;;; Yet To-Do:
;;  - Document perl-tab-to-comment variable.
;;  - advise perl-indent to check for system command?
;;  - Code simplification:
;;    - ?Fragments -> Does gre support them?
;;    - ~/.grerc   -> Does gre use one?
;;  - Learn gre variables
;;    - Display help about variable.
;;    - Display one-line description and default value in minibuffer.
;;      (under point, and under mouse commands)
;;      (Do I have a free mouse key to use?)
;;    - Display all parameters' descriptions and default values in help buffer
;;      (colour-code the display, probably using faces directly rather than 
;;       font-lock).
;;  - See FIXME in gre-apropos for neat enhancements.
;;  
;; I'm thinking of neat ways to handle @param and @default descriptors: A
;; quick access command to display the description and default value in the
;; minibuffer for parameter under point (or mouse), and show in the help
;; buffer (with enhanced display, probably colour-coded) the descriptions for
;; all parameters of the command on the current line.
;;
;; If command contains the word `default', then I should always display
;; it's default value after completion.
;;
;; ~/.gre-VERSION old format: 
;;  `-'   user commands       (gre-complete)
;;  `--'  fragments           (gre-apropos) (gre-help 2)
;;  `---' gre system commands (gre-help 1)
;; 
;;  new format:
;;  `-'   system variables (possibly just to make version-specific alist)
;;  `--'  user commands and fragment headers from ~/.grerc whenever supported
;;         (added at load time)
;;                               (gre-complete)
;;  `---' gre system commands    (gre-help 1)
;;
;;
;;; Gre system and user commands, user fragments and system variables
;;
;; System commands
;;      gre-complete           [syntax-buffer]
;;      gre-apropos            [syntax-buffer]
;;      gre-help-this-command  [syntax-buffer -> gre.cmd]
;;      gre-info-this-command
;;      gre-help               [gre-system-commands-alist-VERSION -> gre.cmd]
;;      gre-info
;;
;; System variables
;;      gre-complete           [syntax-buffer]
;;      gre-apropos            [syntax-buffer]
;;      gre-help-this-command  [syntax-buffer] or [syntax-buffer -> gre.cmd]
;;      gre-help               [gre-system-commands-alist-VERSION] ?-> gre.cmd
;;
;; User commands (read from ~/.grerc and inserted in gre-syntax buffer)
;;      gre-complete           [syntax-buffer]
;;      gre-apropos            [syntax-buffer]
;;    ? gre-help-this-command  [syntax-buffer -> ~/.grerc]
;;    ? gre-help               [gre-user-commands-alist -> ~/.grerc]
;;
;; User commands from current buffer
;;      gre-complete           [current-buffer]
;;
;; User variables from current buffer
;;      gre-complete           [current-buffer]
;;
;; User fragments (read from ~/.grerc and inserted in gre-syntax buffer)
;;      gre-complete           [syntax-buffer -> ~/.grerc]
;;      gre-apropos            [syntax-buffer]
;;      indentation (use advice to indent all inserted lines)
;;      fontification (make sure it's done correctly)
;;
;;; Inverse index:
;;              sys-commands sys-vars user-commands fragments cur-user-cmds var
;; gre-complete:      x         x           x          x           x         x
;; gre-apropos:       x         x           x          x                       
;; gre-help-this-com: x         x           ?
;; gre-help:          x         x           ?
;; gre-info-this-com: x
;; gre-info:          x
;;
;;              sys-commands sys-vars user-commands fragments cur-user-cmds var
;; gre-complete:     done      done        done        x          done     done
;; gre-apropos:      done      done         x          x                       
;; gre-help-this-com:done       x           ?
;; gre-help:         done       x           ?
;; gre-info-this-com:done
;; gre-info:         done
;;
;; ----------------------------------------------------------------------------
;;; Code:
;; The following variable may be edited to suit your site: 

(defvar gre*directory-tree "/usr/local/share/gre"
  "*Root of the gre directory tree.

Root of the gre directory tree holding different versions of gre library
files.  This is either a string, or a list of strings.

This variable must be set correctly for gre-mode to support working with
multiple versions of gre (See commands `gre-set-version' and
`gre-set-local-version').  If left unset, gre-mode will function but will
only use the system default installation of gre, as called from executing
`gre' at the shell prompt.

In the following file layout, gre*directory-tree should be set to 
\"/usr/lib/gre\"

 /usr/bin/gre-2.1.17           The numbered binary (or script) executable
 /usr/lib/gre/2.1.17/gre.cmd   The gre.cmd file for version 2.1.17

In the next file layout, gre*directory-tree should be set to \"/opt/gre\"

 /usr/bin/gre-2.1.17 -> /opt/gre/2.1.17/bin/gre
 /opt/gre/2.1.17/bin/gre       The gre binary for version 2.1.17
 /opt/gre/2.1.17/lib/gre.cmd   The gre.cmd file for version 2.1.17

If you had both layouts, you'd make a list of the directories:

 (setq gre*directory-tree '(\"/opt/gre/\" \"/usr/lib/gre\"))

Notes:
 1 - The lib/ directory is optional.
 2 - The bin/ directory may optionally exist to hold the gre binary.  
     If it doesn't exist, gre-mode will assume that the gre command 
     suffixed with the version number exists in the path 
     (e.g. /usr/bin/gre-2.1.17).

See the gre-mode.el file itself for more information.")

(defvar gre*view-after-run t
  "*When set to true, gre-run will call gre-view after successful completion.
If you do not wish this behaviour, reset it in your .emacs file like so:

  (setq gre*view-after-run nil)")

(defvar gre*view-command "gv" 
  "*command used by gre-view to preview postscript file.
Reset this in your .emacs file (but not in your gre-mode-hook) like so:

  (setq gre*view-command \"ghostview -magstep -1\") ;for small screens
or
  (setq gre*view-command \"gv -media letter -scale 2\")")

(defvar gre*view-landscape-arg "-landscape" 
  "*argument used to obtain landscape orientation in gre-view.
This argument is passed to the shell along with the command stored 
in the variable gre*view-command.
Reset this in your .emacs file (but not in your gre-mode-hook) like so:

  (setq gre*view-landscape-arg \"\")

  where the empty string is used here (as an example) if no landscape
  argument exists for the command used in gre*view-command.")

(defvar gre*view-scale "0"
  "Default scale arguument to use when using gv as gre-view command.
Changed via menu-bar.")
(make-variable-buffer-local 'gre*view-scale)

(defvar gre*lpr-command lpr-command
  "*Command used by gre-mode to print PostScript files produced by gre.
Set only the command name here.  Options are set in gre-print-switches")

(defvar gre*lpr-switches lpr-switches
  "*Options used to print PostScript files produced by gre.
This is usually entered as a list of strings:
   (setq gre*lpr-switches '(\"-P las_images\" \"-h\"))
but can also be entered simply as a single string:
   (setq gre*lpr-switches \"-P las_images -h\")")

(defvar gre*WWW-program nil
  "*Program name for World-Wide-Web browser, used by command gre-WWW-manual.
If set to nil, gre-mode will use the Emacs' browse-url package to deal with
the browser request.  If set to a string, gre-mode will start it as a
sub-process.

On your system, this could be `netscape'.  If so, set this variable in your
.emacs file like so:

  (setq gre*WWW-program \"netscape\")")

(defvar gre*WWW-page "http://www.phys.ocean.dal.ca/~kelley/gre/gre1.html"
  "*Web page or local html index file for the gre manual.
This is used by the gre-WWW-manual command.
On your system, this could be reset to a local html file, e.g.
 (setq gre*WWW-page \"file:/usr/doc/gre/html/index.html\")
but it defaults to the gre web page: 
 http://www.phys.ocean.dal.ca/~kelley/gre/gre1.html

See also:  variable gre*WWW-program.")

(defvar gre*use-imenu (not (string-match "XEmacs\\|Lucid" emacs-version))
  "*Use imenu package for gre-mode?")

;; FIXME: Delete this?
(defvar gre*command-arguments ""
  "gre command arguments to pass to gre when using gre-run, excluding -b -y
which are always sent.")
(make-variable-buffer-local 'gre*command-arguments) 

;;; End of user code. --- No need to read beyond this point.

;; I need `push' for imenu stuff, and I'll use `when' and `unless'
(eval-when-compile (require 'cl))

(if (not (fboundp 'perl-indent-line))   ; perl-mode does not provide itself...
    (load-library "perl-mode"))         ;  Indentation based on perl-mode
                                        ;  Does gre-mode need to be GPL'ed?
(defvar gre-mode-is-XEmacs
  (not (null (save-match-data (string-match "XEmacs\\|Lucid" emacs-version)))))

(defvar gre-mode-is-Emacs20 (= 20 emacs-major-version))

(defvar gre-version nil
  "gre version to use on this file.
You may set this globally using `gre-set-version', or for one file only
using `gre-set-local-version'.
If nil, then the system default version will be used by simply using the
gre command at the shell prompt.  The version number will be stored in 
the variable `gre-system-version'

The command `gre-set-local-version' sets up an Emacs buffer-local variable 
in your gre file by inserting strings like this at the end of the file:

# Local Variables:
# gre-version: \"2.1.1\"
# End:")


(defvar gre-system-version nil
  "Default version of gre installed on the system.
Do not set this variable, gre-mode does so automatically when invoked.")

(defvar gre-version-list nil
  "Internal list of gre versions available on system, from gre*directory-tree")

;;;Method for setting which version of Gre to use on a given buffer:
;; 
;;  gre-mode uses the executable version as set in the Emacs variable
;;  `gre-version', and, like any other Emacs variable, the buffer-local
;;  value takes precedence over the default value.
;;
;;  gre-mode starts up and sets default-value of variable `gre-version' from:
;; 
;;   1- contents of ~/.gre-default-version.
;;      If "VERSION", use /opt/gre/VERSION/bin/gre.
;;      -> `gre-set-version' command sets default-value of `gre-version' 
;;          variable and writes it to ~/.gre-default-version file.
;; 
;;   2- If no ~/.gre-default-version exists, set `gre-version' to nil.
;;      Use system's default version /opt/gre/bin/gre and chase its link
;;      to /opt/gre/VERSION/bin/gre  (Use /opt/gre/ unless gre*directory_tree
;;      is set to something else). 
;; 
;;  gre-mode gets local value of `gre-version' from:
;; 
;;   1- buffer-local variable.
;;      -> `gre-set-local-version' command sets it.
;; 
;;  A separate file like ~/.gre-2.0.0 will be created for each gre
;;  version when a local version is set by the gre-set-local-version command 
;;  (and subsequent emacs local variable).
;;  The creation date of the syntax file is always compared against the 
;;  gre.cmd file to keep it up to date.
;;
;;  When a user uses gre-set-version, then the version number is written to 
;;  ~/.gre-default-version file, and the same syntax file as for a locally
;;  defined version (as above) is used.
;;
;;  When the user sets the version to use to the `system default' with
;;  gre-set-version (setting the value to nil and deleting the
;;  ~/.gre-default-version file, or suppose the user has never used
;;  gre-set-version and created such a file to begin with) then the file
;;  /opt/gre/bin/gre is chased down links to obtain the version number.
;;  Then the appropriate ~/.gre-VERSION file is used.

(defvar gre-mode-syntax-table nil
  "Syntax table in use in gre-mode buffers.")
(if gre-mode-syntax-table
    ()
  (setq gre-mode-syntax-table (make-syntax-table))
  (modify-syntax-entry ?#  "<"  gre-mode-syntax-table) ; # Comments
  (modify-syntax-entry ?\n ">"  gre-mode-syntax-table) ; # Comments
  ;; Give CR the same syntax as newline, for selective-display
  (modify-syntax-entry ?\^m ">" gre-mode-syntax-table) ; # Comments
  (modify-syntax-entry ?$  "/"  gre-mode-syntax-table) ; $ Quotes next char?
;;(modify-syntax-entry ?\' "\"" gre-mode-syntax-table) ; ' delimits a string?
;;(modify-syntax-entry ?`  "\"" gre-mode-syntax-table) ; ` delimits a string?
  (modify-syntax-entry ?\\ "\\" gre-mode-syntax-table) ; \ escapes next char?
  (modify-syntax-entry ?_  "w"  gre-mode-syntax-table) ; _ word constituent
  (modify-syntax-entry ?.  "w"  gre-mode-syntax-table) ; . word constituent
  (modify-syntax-entry ?%  "."  gre-mode-syntax-table)
  (modify-syntax-entry ?&  "."  gre-mode-syntax-table)
  (modify-syntax-entry ?*  "."  gre-mode-syntax-table)
  (modify-syntax-entry ?+  "."  gre-mode-syntax-table)
  (modify-syntax-entry ?-  "."  gre-mode-syntax-table)
  (modify-syntax-entry ?/  "."  gre-mode-syntax-table)
  (modify-syntax-entry ?<  "."  gre-mode-syntax-table)
  (modify-syntax-entry ?=  "."  gre-mode-syntax-table)
  (modify-syntax-entry ?>  "."  gre-mode-syntax-table)
  (modify-syntax-entry ?|  "."  gre-mode-syntax-table))

;; Abbrev Table
(defvar gre-mode-abbrev-table nil
  "Abbrev table used in gre-mode buffers.")
(define-abbrev-table 'gre-mode-abbrev-table ())

;; Mode Map
(defvar gre-mode-map ()
  "Keymap used in gre-mode.")

(if gre-mode-map
    ()
  (setq gre-mode-map (make-sparse-keymap))

  (define-key gre-mode-map "{" 'electric-perl-terminator) 
  (define-key gre-mode-map "\C-c\C-f" 'gre-function-skeleton)
  (define-key gre-mode-map "\C-c\M-h" 'gre-hide-function)
  (define-key gre-mode-map "\C-c\M-H" 'gre-hide-all)
  (define-key gre-mode-map "\C-c\M-s" 'gre-show-function)
  (define-key gre-mode-map "\C-c\M-S" 'gre-show-all)
  (define-key gre-mode-map "\C-c\M-n" 'gre-narrow-to-function)
  (define-key gre-mode-map "\C-c\C-x" 'gre-insert-file-as-comment)
  (define-key gre-mode-map "\C-c\C-v" 'gre-view)
  (define-key gre-mode-map "\C-c\C-r" 'gre-run)
;  (define-key gre-mode-map "\C-c\C-c\C-r" 'tgre-run)
  (define-key gre-mode-map "\C-c\C-p" 'gre-print)
  (define-key gre-mode-map "\C-c\M-v" 'gre-set-version)
 ; Setting this makes the C-u undefined as a prefix key.
 ;(define-key gre-mode-map "\C-u\C-c\M-v" 'gre-set-local-version)
  (define-key gre-mode-map "\C-c\C-a" 'gre-apropos)
  (define-key gre-mode-map "\C-c\C-n" 'gre-next-option)
  (define-key gre-mode-map "\C-c\C-k" 'gre-kill-option)
  (define-key gre-mode-map "\C-c\C-o" 'gre-option-select)
  (define-key gre-mode-map "\C-c\C-d" 'gre-display-syntax)
  (define-key gre-mode-map "\C-c\C-w" 'gre-WWW-manual)
  (define-key gre-mode-map "\C-c\C-i" 'gre-info-this-command)
  (define-key gre-mode-map "\C-c\C-h" 'gre-help-this-command)
 ;(define-key gre-mode-map "\C-u\C-c\C-i" 'gre-info)
 ;(define-key gre-mode-map "\C-u\C-c\C-h" 'gre-help)
  (define-key gre-mode-map "\M-\t"    'gre-complete)
  (define-key gre-mode-map "\C-c?"    'describe-mode)
  (cond 
   ((string-match "XEmacs\\|Lucid" emacs-version)
    (define-key gre-mode-map [(shift button1)] 'gre-option-select-mouse)
    (define-key gre-mode-map [(shift button2)] 'gre-kill-option-mouse))
   (window-system
    ;; Note [S-down-mouse-1] because of emacs-19.30 !!!
    ;; It has a font-selection function there, so [S-mouse-1] won't work.
    (define-key gre-mode-map [S-down-mouse-1] 'gre-option-select-mouse)
    (define-key gre-mode-map [S-mouse-2] 'gre-kill-option-mouse))))

(setq gre-specials-topics
      '(("while" "(gre)While and Until")
        ("if" "(gre)Conditional statements")
;        ("localSynonyms" "(gre)Local Synonyms")
;        ("synonyms" "(gre)About Synonyms")
;        ("builtInVariables" "(gre)Built-in Variables")
        ("user variables" "(gre)User Variables")
        ("math" "(gre)Mathematics")
;        ("columns" "(gre)Manipulating Columns Etc")
        ("concept index" "(gre)Concept Index")))

(let ((topics gre-specials-topics))
  (while topics
    (let* ((topic (car topics))
	   value menu name)
      (setq topics (cdr topics))
      (setq value (nth 0 topic)
            menu (nth 1 topic))
      (setq name (intern (concat "gre-info-" value)))
      (fset name (list 'lambda () 
                       (list 'interactive)
                       (list 'require ''info)
                       (list 'Info-goto-node menu))))
;; In latex.el, Per even builds the menu at this stage!
    ))

(defun gre-add-menu-bar ()
  "Adds the menus to the menu bar in gre-mode."
  (easy-menu-define 
   gre-mode-menu3 gre-mode-map "Menu keymap for gre mode."
   '("Gre-Help"
     ["Help about current command"          gre-help-this-command t]
     ["Help about any command"              gre-help 
      :keys "C-u C-c C-h" :active t]
     ["Info about current command"          gre-info-this-command t]
     ["Info about any command"              gre-info 
       :keys "C-u C-c C-i" :active t]
     ("InfoTopics"
      ["While Statements"                   gre-info-while t]
      ["If Statements"                      gre-info-if t]
      ["Local Synonyms"                     gre-info-localSynonyms t]
      ["About Synonyms"                     gre-info-synonyms t]
      ["Built-in Variables"                 gre-info-builtInVariables t]
      ["About Variables"                    gre-info-variables t]
      ["Reverse Polish Math (rpn stuff)"    gre-info-rpn t]
      ["Manipulating Columns"               gre-info-columns t]
      ["Concept Index"                      gre-info-conceptIndex t]
      )
     ["List gre commands containing string" gre-apropos t]
     ["Display syntax for current command"  gre-display-syntax t]
     ["Gre Manual on WWW"                   gre-WWW-manual t]))
;;;  (easy-menu-define 
;;;   gre-mode-menu2 gre-mode-map "Menu keymap for gre mode."
;;;   '("Hide/Show"
;;;	["Hide this gre function"        gre-hide-function t]
;;;	["Hide all gre functions"        gre-hide-all t]
;;;	["Show this gre function"        gre-show-function t]
;;;	["Show all gre functions"        gre-show-all t]
;;;	["Restrict editing to function"  gre-narrow-to-function t]
;;;	["Remove function restriction"   widen t]))
  (cond
   (gre-mode-is-XEmacs
    (easy-menu-define
     gre-mode-menu1 gre-mode-map "Menu keymap for gre mode."
     '("Gre"
       ;;FIXME: add prefix to *not* view.
       ["Save and Run gre"             (gre-run t) :keys "C-u C-c C-r"]
       ["Save, Run and View gre"        gre-run t]
       ["View existing PostScript"      gre-view  t]
       ("gv scale selection"
;;; this active switch doesn't work in XEmacs
;;      :active (equal gre*view-command "gv")
        ["0.1" 
         (list 'lambda () (interactive)(setq gre*view-scale "-5"))
         :style radio :selected (equal gre*view-scale "-5")]
        ["0.125" 
         (list 'lambda () (interactive)(setq gre*view-scale "-4"))
         :style radio :selected (equal gre*view-scale "-4")]
        ["0.25" 
         (list 'lambda () (interactive)(setq gre*view-scale "-3"))
         :style radio :selected (equal gre*view-scale "-3")]
        ["0.5" 
         (list 'lambda () (interactive)(setq gre*view-scale "-2"))
         :style radio :selected (equal gre*view-scale "-2")]
        ["0.707" 
         (list 'lambda () (interactive)(setq gre*view-scale "-1"))
         :style radio :selected (equal gre*view-scale "-1")]
        ["1" 
         (list 'lambda () (interactive)(setq gre*view-scale "0"))
         :style radio :selected (equal gre*view-scale "0")]
        ["1.414" 
         (list 'lambda () (interactive)(setq gre*view-scale "1"))
         :style radio :selected (equal gre*view-scale "1")]
        ["2" 
         (list 'lambda () (interactive)(setq gre*view-scale "2"))
         :style radio :selected (equal gre*view-scale "2")]
        ["4" 
         (list 'lambda () (interactive)(setq gre*view-scale "3"))
         :style radio :selected (equal gre*view-scale "3")]
        ["8" 
         (list 'lambda () (interactive)(setq gre*view-scale "4"))
         :style radio :selected (equal gre*view-scale "4")]
        ["10" 
         (list 'lambda () (interactive)(setq gre*view-scale "5"))
         :style radio :selected (equal gre*view-scale "5")]
        )
       ["Print existing PostScript"     gre-print t]
       "- Syntax -" 
       ["Complete the gre command"      gre-complete t]
       ["Select option under point"     gre-option-select t]
       ["Kill option under point"       gre-kill-option t]
       ["Go to next option"             gre-next-option t]
       "- Format/Edit -"
       ["Indent current line"           indent-for-tab-command t]
       ["Indent selected region"        indent-region t]
       ["Comment-out region"            gre-comment-out-region t]
       ["Uncomment-out region"          gre-uncomment-out-region t]
       ["Insert file head as a comment" gre-insert-file-as-comment t]
       ["Create function skeleton"      gre-function-skeleton t]
       "- Version in Use- "
       ["Set gre version to use locally" gre-set-local-version
        :keys "C-u C-c M-v" :active t]
       ["Set gre version to use globally" gre-set-version t])))
   (t
    (easy-menu-define
     gre-mode-menu1 gre-mode-map "Menu keymap for gre mode."
     '("Gre"
       ;;FIXME: add prefix to *not* view.
       ["Save and Run gre"             (gre-run t) :keys "C-u C-c C-r"]
       ["Save, Run and View gre"        gre-run t]
       ["View existing PostScript"      gre-view  t]
       ("gv scale selection"
        :active (equal gre*view-command "gv")
        ["0.1" 
         (list 'lambda () (interactive)(setq gre*view-scale "-5"))
         :active (not (equal gre*view-scale "-5"))]
        ["0.125" 
         (list 'lambda () (interactive)(setq gre*view-scale "-4"))
         :active (not (equal gre*view-scale "-4"))]
        ["0.25" 
         (list 'lambda () (interactive)(setq gre*view-scale "-3"))
         :active (not (equal gre*view-scale "-3"))]
        ["0.5" 
         (list 'lambda () (interactive)(setq gre*view-scale "-2"))
         :active (not (equal gre*view-scale "-2"))]
        ["0.707" 
         (list 'lambda () (interactive)(setq gre*view-scale "-1"))
         :active (not (equal gre*view-scale "-1"))]
        ["1" 
         (list 'lambda () (interactive)(setq gre*view-scale "0"))
         :active (not (equal gre*view-scale "0"))]
        ["1.414" 
         (list 'lambda () (interactive)(setq gre*view-scale "1"))
         :active (not (equal gre*view-scale "1"))]
        ["2" 
         (list 'lambda () (interactive)(setq gre*view-scale "2"))
         :active (not (equal gre*view-scale "2"))]
        ["4" 
         (list 'lambda () (interactive)(setq gre*view-scale "3"))
         :active (not (equal gre*view-scale "3"))]
        ["8" 
         (list 'lambda () (interactive)(setq gre*view-scale "4"))
         :active (not (equal gre*view-scale "4"))]
        ["10" 
         (list 'lambda () (interactive)(setq gre*view-scale "5"))
         :active (not (equal gre*view-scale "5"))]
        )
       ["Print existing PostScript"     gre-print t]
       "- Syntax -" 
       ["Complete the gre command"      gre-complete t]
       ["Select option under point"     gre-option-select t]
       ["Kill option under point"       gre-kill-option t]
       ["Go to next option"             gre-next-option t]
       "- Format/Edit -"
       ["Indent current line"           indent-for-tab-command t]
       ["Indent selected region"        indent-region t]
       ["Comment-out region"            gre-comment-out-region t]
       ["Uncomment-out region"          gre-uncomment-out-region t]
       ["Insert file head as a comment" gre-insert-file-as-comment t]
       ["Create function skeleton"      gre-function-skeleton t]
       "- Version in Use- "
       ["Set gre version to use locally" gre-set-local-version
        :keys "C-u C-c M-v" :active t]
       ["Set gre version to use globally" gre-set-version t])))))

(if (or gre-mode-is-XEmacs
        (or window-system
            (fboundp 'tmm-menubar)))
  (gre-add-menu-bar))

;; Gre Mode
(defun gre-mode ()
  "Major mode for editing and running Gre files. 
V1.00 (c) April 23, 1997 --  Peter Galbraith <Galbraith@mixing.Qc.DFO.ca>
COMMANDS AND DEFAULT KEY BINDINGS:
   gre-mode                           Enter Gre major mode.
 Running Gre; viewing output:
   gre-run               C-c C-r      Run gre on this file, and view result.
   gre-view              C-c C-v      Run ghostview on the existing .ps file.
   gre-print             C-c C-p      Print gre .ps file.
 To insert and edit full syntax:
   gre-complete          M-Tab        Complete abbreviated gre command.
   gre-next-option       C-c C-n      Goto to next option, string or variable.
   gre-kill-option       C-c C-k      Deletes gre syntax within brackets.
   gre-option-select     C-c C-o      Selects gre optional syntax.
 To obtain help or find commands:
   gre-apropos           C-c C-a      Display commands containing keyword.
   gre-display-syntax    C-c C-d      Display syntax for command on point.
   gre-help-this-command C-c C-h      Help about user/system command on point.
   gre-help          C-u C-c C-h      Help about prompted-for command.
   gre-info-this-command C-c C-i      Info about system command on point.
   gre-info          C-u C-c C-i      Info about prompted-for command.
   gre-WWW-manual        C-c C-w      World Wide Web gre manual.
   describe-mode         C-c ?        Displays help about gre mode.
 Dealing with multiple installed versions of Gre:
   gre-set-version       C-c M-v      Set gre version to use globally.
   gre-set-local-version C-u M-c C-v  Set gre version to use on this file only.
 Indenting/formatting gre code:
   gre-function-skeleton C-c C-f      Add skeleton for a new function. 
   gre-narrow-to-function 
                         C-c M-n      Narrow editing region to function.
   widen                 C-x n w      Widens editing region back to normal.
   gre-insert-file-as-comment 
                         C-c C-x      Insert filename under point as a comment.
   indent-for-tab        Tab          Indent line for structure.
   indent-region         M-C-\        Indent all lines between point and mark.
   gre-comment-out-region             To avoid running a block of code.
   gre-uncomment-out-region           To undo previous command.
 To use hs-minor-mode to hide gre functions:
   gre-hide-function     C-c M-h      Hides the gre function under point.
   gre-hide-all          C-c M-H      Hides all gre functions in buffer.
                     C-u C-c M-h       (ESC 1 prefixes a true argument)
   gre-show-function     C-c M-s      Shows the gre function on current line.
   gre-show-all          C-c M-S      Shows all gre functions in buffer.
                     C-u C-c M-s       

VARIABLES:
  gre*directory-tree                New path to gre installation with versions.
  gre*view-after-run                t or nil, view ps file after gre-run if t.
  gre*view-command                  String for shell command used by gre-view.
  gre*view-landscape-arg            String for landscape argument in gre-view. 
  gre*WWW-program                   String for local WWW browser program.
  gre*use-imenu                     Use imenu package for gre-mode?
ACCESSING:  
To add automatic support put something like the following in your .emacs file:
  (autoload 'gre-mode \"gre-mode\" \"Enter Gre-mode.\" t)
  (setq auto-mode-alist (cons '(\"\\\\.gre$\" . gre-mode) auto-mode-alist))
If gre is installed in a non-standard place, then you'll need something like:
  (setq gre*directory-tree \"/home/opt/gre/\") ;Path to our gre installation
 See C-h v gre*directory-tree to find out more.
 
And optionally, customize the mode in your .emacs file:
  (setq gre*lpr-switches \"-P laser\"   ; Select a printer
        gre*view-command \"ghostview -magstep -1\" ;for small screens
        gre*view-after-run nil          ; Don't call gre-view after gre-run.
        gre*WWW-program \"netscape\")   ; Our local WWW browser program
  (add-hook 'gre-mode-hook 'my-gre-mode-hook) ;Hook gets invoked after gre-mode
  (defun my-gre-mode-hook ()
    (turn-on-font-lock)                 ;font-lock-mode
    (hs-minor-mode)                     ;use hs-minor-mode to Hide/Show
    (hs-hide-all))                      ;Hide gre functions on startup

SEE ALSO: help about gre-complete (C-h f gre-complete)

KNOWN BUGS:
   gre-help-this-command
     Can't find help on hidden user commands.
   gre-complete
     *completions* buffer lies: you can't use the mouse to make a selection.
      Completions relies on entire line, not just up to the editing point.
   gre-show-all, gre-hide-all
      May get confused if you have a string which looks like a function
      title in your function's help text (i.e. a line which begins with
      a ` character and ends with a ' character.

POSSIBLE ADDITIONS (at some time...):
   Fix bugs!
   Add mouse support to make selection in *completions* buffer.
   Add mouse support to select and kill options."
  (interactive)
  (kill-all-local-variables)
  (use-local-map gre-mode-map)
  (setq major-mode 'gre-mode)
  (setq mode-name "gre")
  (setq local-abbrev-table gre-mode-abbrev-table)
  (set-syntax-table gre-mode-syntax-table)

  ;; Paragraph definitions
  (make-local-variable 'paragraph-start)
  (setq paragraph-start (concat "^$\\|" page-delimiter))
  (make-local-variable 'paragraph-separate)
  (setq paragraph-separate paragraph-start)
  (make-local-variable 'paragraph-ignore-fill-prefix)
  (setq paragraph-ignore-fill-prefix t)

  ;; Indentation
  ;; `indent-for-tab' `indent-according-to-mode' and `indent-region' will use:
  (make-local-variable 'indent-line-function)
  (setq indent-line-function 'perl-indent-line) ;Use perl-mode !!
  ;;;FIXME -> fill-paragraph (need local fill-paragraph-function)
  ;;(define-key gre-mode-map "\M-q" '...)
  (make-local-variable 'comment-indent-function)
  (setq comment-indent-function 'perl-comment-indent)
  (make-local-variable 'parse-sexp-ignore-comments)
  (setq parse-sexp-ignore-comments t)

  (make-local-variable 'require-final-newline)
  (setq require-final-newline t)

  ;; Comments
  (make-local-variable 'comment-start)
  (setq comment-start "# ")
  (make-local-variable 'comment-end)
  (setq comment-end "")
  (make-local-variable 'comment-column)
  (setq comment-column 32)
  (make-local-variable 'comment-start-skip)  ;Need this for font-lock...
  (setq comment-start-skip "\\(^\\|\\s-\\);?#+ *")

  (cond
   (gre*use-imenu
    (require 'imenu)
    (setq imenu-create-index-function 'imenu--create-gre-index)
 ;;;Instead of setting `imenu-create-index-function', I could set:
 ;; (setq imenu-prev-index-position-function 
 ;;       'gre-imenu-prev-index-position-function)
 ;; (setq imenu-generic-expression
 ;;       '((nil "^cmd \\([^(]+\\)" 1)
 ;;        ("Variables" "^\\($[a-zA-Z_]+\\) [+-/*]?=" 1)))
    (if (or window-system
            (fboundp 'tmm-menubar))
        (imenu-add-to-menubar "Imenu"))))

  ;; Gre-mode's own completion mechanisms
  (make-local-variable 'gre-last-complete-point)
  (setq gre-last-complete-point -1)
  (make-local-variable 'gre-last-complete-command)
  (setq gre-last-complete-command "")
  (make-local-variable 'gre-last-complete-status)
  (setq gre-last-complete-status 0)

  ;; Outlininng and hideshow mode
  ;;(require 'outline)
  ;;(make-local-variable 'outline-regexp)
  (cond 
   ((> emacs-major-version 19)
    (make-local-variable 'line-move-ignore-invisible)
    (setq line-move-ignore-invisible t)

;;;FIXME: Outline support: Delete this?
    (if (fboundp 'add-to-invisibility-spec)
	(add-to-invisibility-spec '(outline . t)))
    (setq outline-regexp "^\\(cmd\\|sub\\).*\n")

    (gre-setup-hs)
    )
   (t

;;;FIXME: Outline support: Delete this?
    (setq outline-regexp "^\\(cmd\\|sub\\).*[\n\^M]")
    (setq selective-display t
          selective-display-ellipses t)
    ))

;;; Add XEmacs menubar 
;;;  (and (boundp 'gre-menubar)
;;;       gre-menubar
;;;       (fboundp 'add-submenu)     ;Insurance for emacs
;;;       (set-buffer-menubar (copy-sequence current-menubar))
;;;       (add-submenu nil gre-menubar))
  (when gre-mode-is-XEmacs
    (require 'easymenu)
    (easy-menu-add gre-mode-menu1)
  ;;;FIXME:  Don't know if hide stuff works in XEmacs
  ;;(easy-menu-add gre-mode-menu2)
    (easy-menu-add gre-mode-menu3)
    (setq mode-popup-menu (cons "Gre Mode" gre-mode-menu1)))

  (gre-setup-font-lock)
  ;; Figure Out what version of gre to use, where to call it
  (gre-initialize-default-version)
  (hack-local-variables)
  (run-hooks 'gre-mode-hook))

;; Various indentation styles:       K&R  BSD  BLK  GNU  LW
;;   perl-indent-level                5    8    0    2    4
;;   perl-continued-statement-offset  5    8    4    2    4
;;   perl-continued-brace-offset      0    0    0    0   -4
;;   perl-brace-offset               -5   -8    0    0    0
;;   perl-brace-imaginary-offset      0    0    4    0    0
;;   perl-label-offset               -5   -8   -2   -2   -2

(defvar perl-nochange ";?#\\|\f\\|\\s(\\|\\(\\w\\|\\s_\\)+:"
  ;;FIXME?  Local value? Do I need this at all?
  ;;  "\\(^\\|\C-m\\)[ \t]*\\(\\\\[^ ]+[ ]+=[ ]+\\)?\\(system\\)"
  "*Lines starting with this regular expression are not auto-indented.")
;;                                                      ^^^^

;;;----------
;;; Gre version stuff

(defun gre-initialize-default-version ()
  "Called from gre-mode only, sets default-value for variable gre-version
(the default gre version to use) from ~/.gre-default-version.
Does not validate that this version actually exists."
  (cond
   ((file-readable-p "~/.gre-default-version")
    (let ((the-buffer (create-file-buffer "~/.gre-default-version")))
      (save-excursion
        (set-buffer the-buffer)
        (insert-file-contents "~/.gre-default-version")
        (goto-char (point-min))
        (set-default 
         'gre-version
         (buffer-substring (point)(progn (end-of-line)(point))))
        (kill-buffer the-buffer))))))

(defun gre-version ()
  "Called from all over to return gre version for this buffer.
Returns the user-specified value of gre-version if set.
If gre-version is nil, get version from gre-system-version variable.
If gre-system-version is nil (not yet determined), set it." 
  (cond
   (gre-version
    gre-version)
   (gre-system-version
    gre-system-version)
   (t
    (gre-expand-versions)
    gre-system-version)))

(defun gre-version-noerror ()
  "Return gre version for this buffer, return nil if in error."
  (let ((the-version))
    (if (condition-case nil
            (setq the-version (gre-version))
          (error))
        the-version
      nil)))

(defun gre-file-path (cmd-flag &optional version)
  "Return path of Gre system file based for gre version currently in use.
(Version of Gre currently in use obtained from (gre-version) function) 
Return `gre' path if CMD-FLAG is nil, 
Return `gre.cmd' path if CMD-FLAG is t,
Return `gre.cmd' directory if CMD-FLAG is 1,"
  (let ((the-version (or version (gre-version))))
    (cond 
     (cmd-flag
      (let ((cmd-file (gre-cmd-file-for-version the-version)))
        (if (eq cmd-flag 1)
            (substring cmd-file 0 (string-match "/gre.cmd$" cmd-file))
          cmd-file)))
     (t
      (gre-bin-file-for-version the-version)))))

(defun gre-set-version (set-local-version)
  "Change the default version of gre used in gre-mode.
If you answer with an empty string, the default version of gre will always be 
used (even after it gets upgraded on your system). If you choose another 
version, gre-mode will remember it between emacs sessions by writing it to the
file ~/.gre-default-version.
See also gre-set-local-version."
;; Sets up variable gre-version
;; Sets up (or deletes) ~/.gre-default-version
  (interactive "P")
  (if set-local-version
      (gre-set-local-version)
    (if (and (local-variable-p 'gre-version)
             (y-or-n-p "Unset locally-set Gre version and proceed? "))
        (gre-unset-local-version))
    (if (local-variable-p 'gre-version)
        ()                              ;Still local; We are done.
      (let* ((table (gre-expand-versions))
             (version (and table        ;Choose if we have possiblities (table)
                           (completing-read 
                            "Gre version number to use: [system default] "
                            table nil 0 nil))))
        (cond 
         ((and version                  ;If we have a version number
               (not (string-equal version ""))
               (not (string-equal version "default")))
          (set-default 'gre-version version)
          ;; Write ~/.gre-default-version
          (let ((the-buffer (create-file-buffer "~/.gre-default-version")))
            (save-excursion
              (set-buffer the-buffer)
              (erase-buffer)
              (insert gre-version)
              (write-file "~/.gre-default-version")
              (kill-buffer the-buffer))))
         (t
          ;;Else set to default value.
          (if (file-exists-p "~/.gre-default-version")
              (delete-file "~/.gre-default-version"))
          (set-default 'gre-version nil)))))))

(defun gre-set-local-version ()
  "Set the version of gre to use on this file only.
This sets an emacs local-variable at the end of the current file as a comment
such that gre-mode will use the gre version specified the next time the file
is edited."
  (interactive)
  (let* ((table (gre-expand-versions))
         (version (and table            ;Choose if we have possiblities (table)
                       (completing-read 
                        "Gre version number to use for this file only: "
                        table nil t nil))))
    (cond 
     ((string-equal version "")
      (message "No version specified.  Exiting."))
     ((string-equal version "default")
      (message 
       "Unsetting local-buffer version and using default version of gre")
      (gre-unset-local-version))
     (t
      (save-excursion
        (goto-char (point-max))
        (if (search-backward "# Local Variables:" nil t)
            (if (re-search-forward "# gre-version: \"\\(.*\\)\"" nil t)
                (progn
                  (goto-char (match-beginning 1))
                  (delete-region (match-beginning 1)(match-end 1))
                  (insert version))
              (end-of-line)
              (insert "\n# gre-version: \"" version "\""))
          (goto-char (point-max))
          (insert "# Local Variables:\n"
                  "# gre-version: \"" version "\"\n"
                  "# End:\n")))
      (make-local-variable 'gre-version)
      (setq gre-version version)))))

(defun gre-unset-local-version ()
  "Unset this buffer's local version of gre and use default value instead"
  (interactive)
  (save-excursion 
    (kill-local-variable 'gre-version)
    (save-restriction
      (widen)
      (goto-char (point-max))
      (when (and (re-search-backward "# Local Variables:" nil t)
                 (re-search-forward  "# gre-version:" nil t))
        (delete-region (progn (beginning-of-line)(point))
                       (progn (forward-line 1) (point)))
        (forward-line -1)
        (if (and (looking-at "# Local Variables:\n")
                 (progn (forward-line 1)(looking-at "# End:")))
            (delete-region (progn (end-of-line)(point))
                           (progn (forward-line -1) (point))))))))

(defun gre-old-expand-versions ()
  "Returns a list of gre versions in directory tree gre*directory-tree"
  (if (or (not (file-readable-p gre*directory-tree))
          (not (file-directory-p gre*directory-tree)))
      (error "Directory %s does not exist."  gre*directory-tree)
    (let ((file-list (cdr (cdr (directory-files gre*directory-tree))))
          ;; The above trickery removes ./ and ../ from the list
          (dir-list)(the-file))
      (while file-list                  
        ;; find directories which have /bin/gre and /lib/gre.cmd files
        (setq the-file (car file-list))
        (if (and 
             (not (string-match "^\\(bin\\|lib\\)$" the-file))
             (file-directory-p
              (expand-file-name the-file gre*directory-tree))
             (file-exists-p (expand-file-name (concat the-file "/bin/gre") 
                                              gre*directory-tree))
             (file-exists-p (expand-file-name (concat the-file "/lib/gre.cmd") 
                                              gre*directory-tree)))
            (setq dir-list (cons the-file dir-list)))
        (setq file-list (cdr file-list)))
      (mapcar 'list (nreverse dir-list)))))

;;; Gre versions...
;;
;; Possible Gre versions are found thusly:
;;
;; - even if gre*directory-tree is nil, inquire to shell what the gre version
;;   is.  Do this for any value of gre*directory-tree is set default version.
;;   Commands like gre-set-version will then only list `default' as the 
;;   installed version.  
;;   - if no gre is installed, make gre-mode still useable.
;;
;; - if gre*directory-tree is a string, search for:
;;   1: `gre*directory-tree`/VERSION/gre.cmd
;;      `gre*directory-tree`/VERSION/gre
;;   2: `gre*directory-tree`/VERSION/gre.cmd
;;       Assume command `gre-VERSION' exists
;;   3: `gre*directory-tree`/VERSION/lib/gre.cmd
;;      `gre*directory-tree`/VERSION/bin/gre
;;   4: `gre*directory-tree`/VERSION/lib/gre.cmd
;;       Assume command `gre-VERSION' exists
;;
;;   Note: Instead of assuming that the command `gre-VERSION' exists, we could
;;         search the PATH for gri and gri-* patterns.
;;
;; - if gre*directory-tree is a list of strings, concatenate all results
;;   from the strings.
;; 
;; The link /opt/gre/bin/gre -> /opt/gre/SOME-VERSION/bin/gre
;; is no longer used.
 
(defun gre-expand-versions ()
  "Returns a list of gre versions in gre*directory-tree, 
either a string or list of strings.
Sets gre-version-list variable."
  (save-excursion
    (cond
     ((listp gre*directory-tree)
      (let ((the-list gre*directory-tree)
            the-tree local-gre-version-list)
        (while the-list
          (setq the-tree (car the-list))
          (setq the-list (cdr the-list))
          (setq local-gre-version-list 
                (append 
                 (gre-expand-versions-this-directory the-tree) 
                 local-gre-version-list)))
        (setq gre-version-list local-gre-version-list)))
     ((stringp gre*directory-tree)
      (setq gre-version-list (gre-expand-versions-this-directory 
                              gre*directory-tree))))
    ;; Add "default" entry using gre -directory_default
    ;; NOTE: gre --directory /some/path -directory_default 
    ;;           -> must return /some/path
    (let* ((default-vsn (gre-inquire-default-version))
           (default-cmd (or (gre-inquire-default-cmd)
                            ;; Old versions of gre can't inquire-default-cmd
                            (and gre-version-list
                                 default-vsn
                                 (gre-file-path 
                                  t default-vsn)))))
      (if (and default-vsn default-cmd)
          (progn
            (setq gre-system-version default-vsn)
            (setq gre-version-list 
                  (append 
                   gre-version-list 
                   (list (list "default" 
                               gre-system-version 
                               "gre" 
                               default-cmd)))))))
    ;; If the above didn't work and there's only one entry in 
    ;; gre-version-list, then make it the default.
    (if (and gre-version-list
             (string-equal "" (mapconcat '(lambda (x)) gre-version-list " "))
             (not (string-equal "default" (car (car gre-version-list)))))
        (let ((default-vsn (car (car gre-version-list)))
              (rest (cdr (car gre-version-list))))
          (setq gre-system-version default-vsn)
          (setq gre-version-list 
                (append 
                 gre-version-list 
                 (nconc '("default") rest)))))
    ;; FIXME: If the above still didn't do it, then see if
    ;; gre*directory-tree is a simple strings and use /opt/gre/bin/gre and
    ;; /opt/gre/lib/gre.cmd if they exist.

    ;; return all version as alist for completion
    (mapcar '(lambda (x) (list (car x))) gre-version-list)))

(defun gre-expand-versions-this-directory (directory-tree)
  "Do gre-expand-versions on a single directory"
  (if (or (not (file-readable-p directory-tree))
          (not (file-directory-p directory-tree)))
      nil
    (let ((file-list (cdr (cdr (directory-files directory-tree))))
          ;; The above trickery removes ./ and ../ from the list
          (local-gre-version-list)(the-file))
      (while file-list                  
        ;; find directories which have /bin/gre and /lib/gre.cmd files
        (setq the-file (car file-list))
        (setq file-list (cdr file-list))
        (cond
         ;; Skip over probable `default' version :-(
         ((string-equal the-file "lib"))
         ;;   1: `gre*directory-tree`/VERSION/gre.cmd
         ;;      `gre*directory-tree`/VERSION/gre
         ((and
           (file-directory-p (expand-file-name the-file directory-tree))
           (file-exists-p (expand-file-name (concat the-file "/gre.cmd") 
                                            directory-tree))
           (file-exists-p (expand-file-name (concat the-file "/gre") 
                                            directory-tree)))
          (setq local-gre-version-list 
                (append local-gre-version-list
                        (list (list the-file the-file
                                    (expand-file-name (concat the-file "/gre") 
                                                      directory-tree)
                                    (expand-file-name (concat the-file 
                                                              "/gre.cmd") 
                                                      directory-tree))))))
         ;;   2: `gre*directory-tree`/VERSION/gri.cmd
         ;;       Assume command `gre-VERSION' exists
         ((and
           (file-directory-p (expand-file-name the-file directory-tree))
           (file-exists-p (expand-file-name (concat the-file "/gre.cmd") 
                                            directory-tree)))
          (setq local-gre-version-list 
                (append local-gre-version-list
                        (list (list the-file the-file
                                    (concat "gre-" the-file)
                                    (expand-file-name (concat the-file 
                                                              "/gre.cmd") 
                                                      directory-tree))))))
         ;;   3: `gre*directory-tree`/VERSION/lib/gri.cmd
         ;;      `gre*directory-tree`/VERSION/bin/gri
         ((and
           (file-directory-p (expand-file-name the-file directory-tree))
           (file-exists-p (expand-file-name (concat the-file "/lib/gre.cmd") 
                                            directory-tree))
           (file-exists-p (expand-file-name (concat the-file "/bin/gre") 
                                            directory-tree)))
          (setq local-gre-version-list 
                (append local-gre-version-list
                        (list (list the-file the-file
                                    (expand-file-name (concat the-file 
                                                              "/bin/gre") 
                                                      directory-tree)
                                    (expand-file-name (concat the-file 
                                                              "/lib/gre.cmd") 
                                                      directory-tree))))))
         ;;   4: `gre*directory-tree`/VERSION/lib/gri.cmd
         ;;       Assume command `gre-VERSION' exists
         ((and
           (file-directory-p (expand-file-name the-file directory-tree))
           (file-exists-p (expand-file-name (concat the-file "/gre.cmd") 
                                            directory-tree)))
          (setq local-gre-version-list 
                (append local-gre-version-list
                        (list (list the-file 
                                    (concat "gre-" the-file)
                                    (expand-file-name (concat the-file 
                                                              "/gre.cmd") 
                                                      directory-tree))))))))
      local-gre-version-list)))
  
(defun gre-inquire-default-cmd ()
  "Ask gre which -default_directory to use to find gre.cmd."
  (let ((gre-tmp-buffer (get-buffer-create "*gre-tmp-buffer*"))
        cmd-file)
    (set-buffer gre-tmp-buffer)
    (shell-command-on-region 1 1 "gre --which_library" t)
    (setq cmd-file 
          (and (not (search-backward "ERROR" nil t))
               (not (search-backward "not found" nil t))
               (not (search-backward "Unknown option" nil t))
               (not (search-backward "Wrong number of args on command" nil t))
               (expand-file-name 
                "gre.cmd" 
                (buffer-substring 1 (progn 
                                      (goto-char 1)(end-of-line)(point))))))
    (kill-buffer gre-tmp-buffer)
    cmd-file))

(defun gre-inquire-default-version ()
  "Ask gre -v to find version number"
  (let ((gre-tmp-buffer (get-buffer-create "*gre-tmp-buffer*"))
        answer)
    (set-buffer gre-tmp-buffer)
    (shell-command-on-region 1 1 "gre -v" t)
    (goto-char (point-min))
    (if (re-search-forward "re version \\(.*\\)$" nil t)
        (setq answer (match-string 1)))
    (kill-buffer gre-tmp-buffer)
    answer))

(defun gre-bin-file-for-version (version)
  (if (not gre-version-list)
      (gre-expand-versions))
  (let ((the-list gre-version-list)
        answer)
    (while the-list
      (if (string-equal version (elt (car the-list) 1))
          (progn
            (setq answer (elt (car the-list) 2))
            (setq the-list nil)))
      (setq the-list (cdr the-list)))
    answer))

(defun gre-cmd-file-for-version (version)
  (if (not gre-version-list)
      (gre-expand-versions))
  (let ((the-list gre-version-list)
        answer)
    (while the-list
      (if (string-equal version (elt (car the-list) 1))
          (progn
            (setq answer (elt (car the-list) 3))
            (setq the-list nil)))
      (setq the-list (cdr the-list)))
    answer))

;;;----------
;;; Running gre functions

(defun gre-run (&optional inhibit-gre-view)
  "Save buffer to a file, then run Gre on it, creating a PostScript file
called FILE.ps where FILE is the basename of the gre command-file.

After its successful completion, gre-run will invoke gre-view if the
variable gre*view-after-run is set to true.  If gre-run ends in error,
it will try to place the edit point on the source line which contains 
the error.  

If an optional prefix argument is supplied to gre-run, gre-view will not 
be run."
  (interactive "P")
  (if (buffer-modified-p)
      (save-buffer))
  (let* ((filename buffer-file-name)
         (shortname (file-name-nondirectory filename))
         (version (or (gre-version)
		      (error "gre-mode doesn't know what gre to use")))
	 (gre-bin-file (gre-file-path nil version))
	 (gre-cmd-file (gre-file-path t version)))
    (if (or (not gre-bin-file) (not gre-cmd-file))
        (error "Sorry, cannot find system files for Gre version %s" version))
    (if (or (and (not (string-equal "gre" gre-bin-file))
		 (not (file-executable-p gre-bin-file)))
            (not (file-exists-p gre-cmd-file)))
        (error "Sorry, cannot execute %s or find %s" gre-bin-file gre-cmd-file))
    (cond
     (gre-version
      (message "Running gre version %s on %s" version shortname))
     (t
      (message "Running default gre %s on %s" version shortname)))
    (shell-command 
     (concat gre-bin-file
             " -L " (gre-file-path 1 version) " "
             gre*command-arguments " " 
             shortname))
    (if (not (get-buffer "*Shell Command Output*"))
        (progn
          (if (and gre*view-after-run
                   (not inhibit-gre-view))
              (gre-view))
          (message "Gre command completed with no output."))
      ;; There is a shell output buffer...
      (let ((display-buffer-p)(msg)(eline)(efile))
        (save-excursion
          (set-buffer "*Shell Command Output*")
          (goto-char (point-min))
          ;;test2.gre:10: Unknown command `set trace on set x size'
          (if (re-search-forward 
               (concat "^" shortname ":\\([0-9]+\\): *\\(.*\\)")
               nil t)
              (progn
                (setq efile filename
                      eline (string-to-int 
                             (buffer-substring
                              (match-beginning 1) (match-end 1)))
                      msg (format "%s at line %s"
                                  (buffer-substring 
                                   (match-beginning 2) (match-end 2))
                                  (buffer-substring
                                   (match-beginning 1) (match-end 1))))))
                
          (if (re-search-forward "^Segmentation fault" nil t)
              (setq msg "Segmentation Fault while running gre!"))
          ;;FATAL error: gr.m:2352: can't use negative y (0.00000) with LOG
          (if (re-search-forward "^\\(ERROR:\\|FATAL error:\\).*$" nil t)
              (setq msg (buffer-substring (match-beginning 0)(match-end 0))))
          (if (re-search-forward "^PROPER USAGE:" nil t)
              (setq display-buffer-p t))
          ;;Error detected at /home/rhogee/new/paper/enlarged_map.gre:42
          (if (re-search-forward 
               "Error detected at \\([^:]+\\):\\([0-9]+\\)" nil t)
              (setq efile (buffer-substring (match-beginning 1)(match-end 1))
                    eline (string-to-int 
                           (buffer-substring(match-beginning 2)(match-end 2)))))
          (goto-char (point-min))
          (set-window-point (get-buffer-window (current-buffer))
                            (point-max))) ;This won't work !!!
        (cond 
         ((or efile msg display-buffer-p)
          (when efile 
            (find-file efile)
            (goto-line eline))
          (if display-buffer-p (display-buffer "*Shell Command Output*"))
          (if msg (error msg)))
         (t
          ;;Clean execution
          (let ((lines
                 (save-excursion
                   (if (not (get-buffer "*Shell Command Output*"))
                       0
                     (set-buffer "*Shell Command Output*")
                     (if (= (buffer-size) 0)
                         0
                       (count-lines (point-min) (point-max)))))))
            (if (and gre*view-after-run
                     (not inhibit-gre-view))
                (gre-view))
            (cond ((= lines 0)
                   (message "Gre command completed with no output.")
                   (kill-buffer "*Shell Command Output*"))
                  ;; If there was only one line of output from Gre, we should
                  ;; repeat it here.
                  ((= lines 1)
                   (message "%s"
                            (save-excursion
                              (set-buffer  "*Shell Command Output*")
                              (goto-char (point-min))
                              (buffer-substring 
                               (point)
                               (progn (end-of-line)(point))))))))))))))

(defun gre-view ()
  "Run X-windows viewer in lower shell on existing .ps file for the gre buffer.
Use landscape mode if `set page lansdscape' is found in gre buffer.
A gzip'ed postscript file can also be viewed without overwriting the file.

Set variables gre*view-command and gre*view-landscape-arg in your .emacs
file to control what commands are sent to the shell to display the
postscript file.  Default values are: 

  (setq gre*view-command \"ghostview\")
  (setq gre*view-landscape-arg \"-landscape\") 

See also the gre-run command, which calls gre-view automatically after
successfully executing your gre buffer if the variable gre*view-after-run 
is set to true."
;;; Asynchronyous output goes to *shell-command* buffer.
;;;  Return windows to original state because that buffer is usually empty
;;;  (if no error occurred) and will probably be empty on error by the time
;;;  the function finishes, because it's asynchronious.
  (interactive)
  (let* ((psfile (concat (gre-filename-sans-suffix buffer-file-name) ".ps"))
         (landscape
          (save-excursion
            (goto-char (point-min))
            (if (re-search-forward "^[ \t]*set[ ]+page[ ]+landscape" nil t)
                (concat " " gre*view-landscape-arg " ")
              "")))
         (scale
          (if (equal gre*view-command "gv")
              (setq scale (concat " -scale " gre*view-scale " "))
            ""))
         (the-command
          (cond 
           ((file-readable-p psfile)
            (concat gre*view-command scale landscape psfile))  
           ((file-readable-p (concat psfile ".gz"))
            (concat "gunzip -c " psfile ".gz | " gre*view-command
                    scale landscape " -"))
           (t
            (error "%s not found or not readable" psfile)))))
    (message the-command)
    (if (string-match "^18" emacs-version)
        (save-window-excursion
          (shell-command the-command))
      ;; Do it Asynchronyously for emacs-19...
      (let ((buffer (get-buffer-create "*gre-view*"))
            (shell-command-switch 
             (or (and (boundp 'shell-command-switch) shell-command-switch)
                 "-c"))
            (directory default-directory)
            proc)
        (save-excursion
          (set-buffer buffer)
          (setq default-directory directory))
        (setq proc (start-process 
                    "Shell" buffer shell-file-name 
                    shell-command-switch the-command))
        (setq mode-line-process '(":%s"))
        (set-process-sentinel proc 'shell-command-sentinel)
        (set-process-filter proc 'gre-insertion-filter)))))

(defun gre-filename-sans-suffix (name)
  "Return FILENAME without .gre suffix.  
If FILENAME does not end in `.gre', return FILENAME."
  (substring name 0
	     (or (string-match ".gre\$" name)
	 (length name))))

;; From elisp Info manual as ordinary-insertion-filter
(defun gre-insertion-filter (proc string)
  (let ((old-buffer (current-buffer)))
    (unwind-protect
        (let (moving)
          (set-buffer (process-buffer proc))
          (setq moving (= (point) (process-mark proc)))
          (save-excursion
            ;; Insert the text, moving the process-marker.
            (goto-char (process-mark proc))
            (insert string)
            (set-marker (process-mark proc) (point)))
          (if moving (goto-char (process-mark proc))))
      (set-buffer old-buffer))))

;;;----------
;;; Syntax Building.

(defun gre-syntax-buffer (where)
  "Return syntax-file buffer for version associated with current buffer, 
creating the syntax buffer if necessary.
If WHERE is 0, go to `-'   to see system variables and all commands
If WHERE is 1, go to `--'  to see user and system commands
If WHERE is 2, go to `---' to see only gre system commands"
  (let* ((version (gre-version))
         (gre-cmd-file (gre-file-path t))
         (gre-syntax-buffer (get-buffer-create (concat "*gre-" version "*")))
         (gre-syntax-file (concat "~/.gre-" version))
         buffer-rebuild-p)
    ;; Check if existing ~/.gre-syntax is up-to-date wrt gre-cmd-file
    (when (or (not (file-readable-p gre-syntax-file))
              (file-newer-than-file-p gre-cmd-file gre-syntax-file)
              (file-newer-than-file-p (file-chase-links gre-cmd-file)
                                      gre-syntax-file))
      (setq buffer-rebuild-p t)
      (gre-build-syntax)) ; build ~/.gre-syntax-VERSION
    (save-excursion
      (set-buffer gre-syntax-buffer)
      (setq buffer-read-only nil)
      (if buffer-rebuild-p (erase-buffer)) ;If the syntax file was out of date.
      (goto-char (point-min))
      ;; Load in ~/.gre-syntax-VERSION if haven't done so yet
      (when (and (= (point-min)(point-max)) (file-readable-p gre-syntax-file))
        (insert-file-contents gre-syntax-file)
        (setq buffer-rebuild-p t))
      (goto-char (point-min))
      (when buffer-rebuild-p
        (message "Using syntax for gre version %s" version)
        ;;FIXME: Is there still a .grerc file?
        (if (file-readable-p "~/.grerc")
            (let ((tmp-buffer (get-buffer-create "*gre-tmp-command*")))
              (save-excursion
                (set-buffer tmp-buffer)
                (insert-file-contents "~/.grerc")
                (gre-add-commands-from-current-buffer nil gre-syntax-buffer))
              (kill-buffer tmp-buffer))))
      (set-buffer-modified-p nil)
      (setq buffer-read-only t)
      (goto-char (point-min))
      (if (= 0 where)
          (re-search-forward "^-\n" nil t)
        (if (= 1 where)
            (re-search-forward "^--\n" nil t)
          (re-search-forward "^---\n" nil t))))
    gre-syntax-buffer))

(defun gre-build-syntax ()
  "Called when gre-syntax-file is not found.  Creates ~/.gre-syntax-VERSION
which contains all commands found in gre.cmd
We must be in gre buffer to call this function to get correct version!"
  (let* ((version (gre-version))
         (syntax-file (expand-file-name (concat "~/.gre-" version)))
         (cmd-file (gre-file-path t))
         (cmd-buffer (get-buffer-create "*gre-cmd-file*"))
         (syntax-buffer (get-buffer-create "*gre-syntax-file*")))
    (save-excursion
      (set-buffer cmd-buffer)
      (insert-file-contents cmd-file)
      (message "Building gre version %s syntax..." version)
      (save-excursion
        (set-buffer syntax-buffer)
        (insert " Syntax for gre version " version 
                "  (created by gre-mode.el)\n"
                " Based on: " (file-chase-links cmd-file) "\n"
                "-\n--\n---\n"))
      (gre-add-commands-from-current-buffer t syntax-buffer)
      (gre-add-variables syntax-buffer)
      (set-buffer syntax-buffer)
      (write-file syntax-file))
    (kill-buffer syntax-buffer)
    (kill-buffer cmd-buffer)))

(defun gre-add-variables (syn-buffer)
  "Add gre variables in current buffer to syntax file.
ARG is gre-syntax buffer to add to."
  (let ((the-list))
    (save-excursion
      (goto-char (point-min))
      (while (re-search-forward "^#\\* @variable \\($_[^ ]*\\)\\(.*$\\)" nil t)
;;;#* @variable $_arrow_size width of arrow heads @unit cm @default 0.2
        (let ((the-variable (match-string 1))
              (the-rest (match-string 2)))
          (setq the-list (cons (list the-variable the-rest) the-list))))
      (set-buffer syn-buffer) 
      (setq buffer-read-only nil)
      (goto-char (point-min))
      (re-search-forward "^-$" nil t)
      (while the-list
        (let ((variable-pair (car the-list)))
          (insert "\n" (car variable-pair) ";" (car (cdr variable-pair)))
          (setq the-list (cdr the-list))))
      (set-buffer-modified-p nil)
      (setq buffer-read-only t))))

(defun gre-add-commands-from-current-buffer (system-flag syn-buffer)
  "Add gre commands in current buffer to syntax file.
If ARG1 is true, then this is the gre.cmd buffer for gre system commands.
ARG2 is gre-syntax buffer."
  (save-excursion
    (set-buffer syn-buffer) 
    (setq buffer-read-only nil)
    (when (not system-flag)
      (goto-char 1)
      (re-search-forward "^---$" nil t)
      (beginning-of-line)
      (narrow-to-region 1 (point))))
  (save-excursion
    (let ((cmd-buffer (current-buffer)))
      (goto-char (point-min))
      (while (re-search-forward "^\\(#\\* @\\)?cmd \\([^(]*\\) (" nil t)
;;;cmd draw box ($$$$;$$) {	# [filled] $ll_x $ll_y $ur_x $ur_y [cm]'
        (let ((the-command (match-string 2))
              (the-options)
              (the-default)
              (the-help-text)
              (limit (save-excursion (end-of-line)(point)))
              (help-limit (save-excursion (beginning-of-line)(point))))
          (search-forward "{" nil t)
          (if (looking-at "[\t ]*#[\t ]*\\(.*\\)")
              (setq the-options (match-string 1)))
          (save-excursion
            (forward-line -1)
            (while (and (looking-at "^#\\*")
                        (= 0 (forward-line -1))))
            (forward-line 1)
            ;; If we have `default' in options, then extract its value.
            (if (and the-options (string-match "default" the-options))
                (save-excursion
                  (when (re-search-forward "@default +\\([a-zA-Z0-9.]+\\)" 
                                         help-limit t)
                    (setq the-default (match-string 1))
                    (beginning-of-line)
                    (if (re-search-forward "@unit +\\([a-zA-Z0-9.]+\\)"
                                           (save-excursion 
                                             (end-of-line)(point))
                                           t)
                        (setq the-default 
                              (concat the-default " "
                                      (buffer-substring-no-properties
                                       (match-beginning 1)
                                       (match-end 1))))))))
            (if (looking-at "^#\\* \\(.+\\)")
                (setq the-help-text (match-string 1))))
          (set-buffer syn-buffer)
          (goto-char (point-max))
          (insert the-command ";" the-command)
          (if the-options (insert " " the-options))
          (insert ";")
          (if the-help-text (insert the-help-text))
          (if the-default (insert " (default is " the-default ")"))
          (insert "\n"))
        (set-buffer cmd-buffer)
        (re-search-forward "^}" nil t)))
    (set-buffer syn-buffer)
    (set-buffer-modified-p nil)
    (widen)
    (setq buffer-read-only t)))

;; (defvar gre-underscore-trans
;;   (let ((string (make-string 256 ? ))
;;         (i 0))
;;     (while (< i 256)
;;       (aset string i i)
;;       (setq i (1+ i)))
;;     (aset string ?   95)
;;     string)
;;   "Trans table from spaces to underscores.")
;; 
;; (defvar gre-underscore2space-trans
;;   (let ((string (make-string 256 ? ))
;;         (i 0))
;;     (while (< i 256)
;;       (aset string i i)
;;       (setq i (1+ i)))
;;     (aset string ?_   32)
;;     string)
;;   "Trans table from underscores to spaces.")

;;; FIXME: Obsolete?  Used by anyone?
(defun gre-obsolete-create-cmd-name ()
  "Edits full gre syntax line to leave gre command name.
Gre command name may be longer than that used by gre parser, because this
will take words that follow variables (and the gre parser won't check these).
Assumes line is last in buffer, and has no leading whitespace."
  (beginning-of-line)
  (while (search-forward "  " nil t)  ;; trim whitesppace
    (delete-backward-char 1)
    (beginning-of-line))
  (while (re-search-forward "[{|\"$[]" nil t)
    (backward-char 1)                 ;; puts point on found character
    (let ((the-match (match-string 0))
          (the-start (point))
          (the-find  (point)))
      (cond
       ((or (string-equal the-match "{") (string-equal the-match "["))
        (goto-char (scan-sexps (point) 1)))  ;; faster than forward-sexp 1?
       ((string-equal the-match "|")
        (skip-chars-backward " \t") ;; place start-deletion before prior option
        (search-backward " " nil t) ;; skipping immediate spaces, find start
        (forward-char 1)            ;; move over first character 
        (setq the-start (point))    ;; set the start-deletion
        (goto-char the-find)        ;; now go back to delete trailing option
        (forward-char 1)            ;; move right after | character
        (skip-chars-forward "\t ")  ;; and skip over spaces
        (re-search-forward "[ {[]\\|$" nil t)
        (if (or (string-equal "{" (match-string 0))
                (string-equal "[" (match-string 0)))
            (backward-char 1)))     ;; don't delete the bracket
       ((string-equal the-match "\"")
        (forward-char 1)
        (search-forward "\"" nil t))
       ((string-equal the-match "$")
        (search-forward " " nil 1)))
      (skip-chars-forward " \t")
      (delete-region the-start (point))
      (when (looking-at "|") ;; add a dummy option, deleted next iteration 
          (save-excursion (insert "dum")))))
  (end-of-line)(insert " ")(backward-char 1)   ;; at least one space at end
  (while (char-equal 32 (char-after (point)))  ;; delete all spaces at end
    (delete-char 1) (backward-char 1)))

;;;----------
;;; Gre command alists and command prompting

;;; How to handle version-specific lists:
;; I use version-specific variables directly:
;;  creating like so:
;;   (set (intern (concat "gre-alist-" (gre-version))) '(("test") ("test2")))
;;
;;  testing like so:
;;   (boundp (intern (concat "gre-alist-" (gre-version))))
;;     =>t
;;  using like so:
;;   (completing-read "test: " 
;;     (symbol-value (intern (concat "gre-alist-" (gre-version)))))
;;
;;; I could have created the alist and stored its symbol in a buffer-local 
;;; variable:
;;
;;  create the version-specific alist:
;;   (set (intern (concat "gre-sys-commands-alist-" (gre-version))) 
;;         '(("test") ("test2")))
;;
;;  make a buffer-local variable to hold the version-specific alist symbol:
;;   (defvar gre-local-sys-commands-alist nil 
;;     "Symbol to use for gre command alist for this buffer")
;;   (make-variable-buffer-local 'gre-local-sys-commands-alist)
;;  
;;  test it:
;;   (boundp 'gre-local-sys-commands-alist)
;;
;;  store the symbol in the buffer-local variable:
;;   (setq gre-local-sys-commands-alist 
;;         (intern (concat "gre-sys-commands-alist-" (gre-version))))
;;
;;  fetch the value with (symbol-value):
;;   (completing-read "test: " (symbol-value gre-local-sys-commands-alist))

(defvar gre-hist-alist nil "Alist to use for gre history")

;; FIXME: are we providing help for user commands from ~/.grerc ?
(defun gre-prompt-for-command (user-flag)
  "Prompt user for gre command name, providing minibuffer completion.
Allow user commands if ARG is t."
  (let ((version (gre-version)))
    (gre-build-sys-commands-alist version)
    (if (not user-flag)
        (completing-read 
         "gre system command: " 
         (symbol-value (intern (concat "gre-sys-commands-alist-" version)))
         nil 0 nil 'gre-hist-alist)
      (gre-build-user-commands-alist)
      (completing-read 
       "gre user or system command: " 
       (append 
        (symbol-value (intern (concat "gre-sys-commands-alist-" version)))
        gre-user-command-alist)
       nil 0 nil 'gre-hist-alist))))

;; FIXME: are we providing help for user commands from ~/.grerc ?
(defvar gre-user-command-alist nil "Alist of gre user commands")

(defun gre-build-user-commands-alist ()
  "Build gre user command alist from ~/.grerc for gre-help"
  (let ((build-list))
    (if (not gre-user-command-alist)
        nil                             ;We're done!
      (save-excursion
        (set-buffer (gre-syntax-buffer 1))
;;; FIXME: This isn't done yet!  Look at ~/.grerc file instead!!!
        (while (not (looking-at "---"))
          (setq gre-user-command-alist
                (nconc gre-user-command-alist
                       (list (list 
                              (buffer-substring 
                               (point)
                               (progn (search-forward ";")
                                      (backward-char 1)(point))))))))))))

(defun gre-build-sys-commands-alist (version)
  "Build alist of gre system commands gre-sys-commands-alist-VERSION"
;;; Could be done using an obarray and `intern' to create it, but you
;;;  can't concatenate obarrays, so this is a problem for user commands.
;;; elisp info doesn't say if completing-read is more efficient with alists
;;;  or obarrays.
  (let ((build-list))
    (if (and (boundp (intern (concat "gre-sys-commands-alist-" version)))
             (symbol-value 
              (intern (concat "gre-sys-commands-alist-" version))))
        nil                             ;We're done!
      (save-excursion
        (set-buffer (gre-syntax-buffer 2))
        (message "building list of gre commands for version %s..." version)
        (forward-line 1)
        (while (< (point) (point-max))
          (setq build-list
                (nconc build-list
                       (list (list (buffer-substring 
                                    (point)
                                    (progn (search-forward ";")
                                           (backward-char 1)(point)))))))
          (forward-line 1)))
      (set (intern (concat "gre-sys-commands-alist-" (gre-version))) 
           build-list))))

(defun gre-build-sys-variables-alist (version)
  "Build alist of gre system variables gre-sys-variables-alist-VERSION"
  (let ((build-list))
    (if (and (boundp (intern (concat "gre-sys-variables-alist-" version)))
             (symbol-value 
              (intern (concat "gre-sys-variables-alist-" version))))
        nil                             ;We're done!
      (save-excursion
        (set-buffer (gre-syntax-buffer 0))
        (message "building list of gre variables for version %s..." version)
        (forward-line 1)
        (while (not (looking-at "^-"))
          (setq build-list
                (nconc build-list
                       (list (list (buffer-substring 
                                    (point)
                                    (progn (search-forward ";")
                                           (backward-char 1)(point)))))))
          (forward-line 1)))
      (set (intern (concat "gre-sys-variables-alist-" (gre-version))) 
           build-list))))

;;;----------
;;; Extracting a (possibly multi-lined) Gre command string from text


;;;FIXME: This could be a lot shorter considering that a command name ends 
;;;       at the first variable.
(defun gre-isolate-this-command ()
  "Return multi-line command as single string.
The command is trimmed and edited to remove any variables, but is not verified.
You can end up with:
draw label at cm"
  (if (save-excursion (beginning-of-line) (looking-at "^[ \t]*$"))
      (error "This is an empty line"))
  (save-excursion
    (let ((here (point))
          (tmp-buffer (get-buffer-create "*gre-tmp-command*"))
          (string) (end-of-buffer 0))
      ;; Point could be after command on a line with a comment
      ;; set x axis 0 1;  [here]       # comment
      (when (looking-at "[ \t]*#")
        (skip-chars-backward " \t")
        (if (= 59 (preceding-char))     ; Character is ";"
            (forward-char -1)))         ; Move within the command.

      ;; Go to beginning of command, after previous ; or { or }
      (re-search-backward "[;{}]" nil 1)
      (forward-char 1)
      (skip-chars-forward " \t")
      (while (looking-at "#\\|$")
          (progn (forward-line 1)(skip-chars-forward " \t")))

      (while (and (= 0 end-of-buffer)
                  (not (looking-at ".*[;{}]")))
        (setq string (concat string (and string " ") 
                             (buffer-substring
                              (progn (skip-chars-forward " \t") (point))
                              (progn (cond ((looking-at ".*#")
                                            (search-forward "#")
                                            (forward-char -1))
                                           (t
                                            (end-of-line)))
                                     (skip-chars-backward " \t")
                                     (point)))))
        (setq end-of-buffer (forward-line 1)))
      (if (looking-at ".*[;{}]")
          (setq string (concat string (and string " ") 
                               (buffer-substring
                                (progn (skip-chars-forward " \t") (point))
                                (progn (re-search-forward "[;{}]")
                                       (forward-char -1)
                                       (skip-chars-backward " \t")
                                       (point))))))
      (set-buffer tmp-buffer)
      (insert string)
      (goto-char 1)
      (if (re-search-forward "[{$;[]" nil 1)
          (forward-char -1))
      (skip-chars-backward " \t")
      (setq string (buffer-substring 1 (point)))
      (kill-buffer tmp-buffer)
      (gre-replace-within-string string "  " " "))))

(defun gre-isolate-this-validated-command (syntax-flag)
  "Shorten COMMAND until can return real command name.
if SYNTAX_FLAG is true, return full syntax instead of just command name."
  (let ((the-command (gre-isolate-this-command))
        ;; the-command could be like draw_label_at_cm
        ;; check in syntax if a command name
        (syntax-buffer (gre-syntax-buffer 0)))
    (save-excursion
      (set-buffer syntax-buffer)
      (let ((the-start (point)))
        (while (and (not (re-search-forward 
                          (concat "^" the-command ";") nil t))
                    (progn (setq the-command 
                                 (gre-shorten-guess-command the-command " "))
                           the-command)))          
        ;; FIXME: Still doing fragments?
        (cond 
         ((= (point) the-start)
          nil)
         (syntax-flag
          (buffer-substring (point) (progn (end-of-line)(point))))
         (t
          (buffer-substring (progn (forward-char -1)(point))
                            (progn (beginning-of-line)(point)))))))))

;;;----------
;;; Showing Help.

(defun gre-display-syntax ()
  "Displays (in minibuffer) the full syntax of the gre command on point.
The gre command may span many line, e.g.

  draw x axis \
     at ..ymargin.. \    <gre-display-syntax> 
        {rpn ..xmargin.. .2 -} cm

  will display
    draw x axis [at bottom|top|{.y. [cm]} [lower|upper]]
  in the minibuffer."
  (interactive)
  (let ((the-command (gre-isolate-this-validated-command t)))
    (if (not the-command)
        (error "Sorry, cannot find such a gre command")
      (message "%s" the-command))))

(defun gre-apropos (keyword)
  "Displays all gre commands containing keyword argument in *help* buffer.
Code fragment abbreviations (e.g. ?set axes) are not included at present time.
The keyword is actually a regular expression;  while this could be a simple 
string, you could also list *all* gre commands with

 gre-apropos .

which matches any character." 
;;; FIXME: Check out apropos.el
;;;  -> Adds a 1-liner of help for each entry.
;;;  -> apropos-print uses (with-output-to-temp-buffer) but standard-ouput
;;;     is in a (let) form!
;;;  -> There's added text for window-system to click on mouse-highlighted
;;;     text for more help. (Uses apropos-print-doc to do it.)
;;;     See apropos-mouse-follow.
;;;  -> Sets the *Apropos* buffer to apropos-mode.  We could have a
;;;     *gre-apropos* buffer and do the same thing.
;;;  -> I should also fontify the buffer as gre-mode!
  (interactive "sgre apropos: ")
  (save-excursion
    (set-buffer (gre-syntax-buffer 0))
    (if (re-search-forward keyword nil t)
        (with-output-to-temp-buffer " *Gre Help*"
          (princ "List of gre commands containing: ")
          (princ  keyword)
          (princ "\n\n")
          (beginning-of-line)
          (cond
           ((looking-at "^\\$")
            (princ "Variable ")
            (princ (buffer-substring (point)
                                     (progn (search-forward ";")
                                            (forward-char -1)(point))))
            (princ (buffer-substring (progn (forward-char 2)(point))
                                     (progn (end-of-line)(point)))))
           (t
            (princ (gre-format-display-command 
                    (buffer-substring (progn (search-forward ";") (point))
                                      (progn (search-forward ";") (point)))))
            (princ (concat "\n  " (buffer-substring 
                                   (point) (progn (end-of-line)(point)))))))
          (forward-line 1)
          (while (re-search-forward keyword nil t)
            (beginning-of-line)
            (princ "\n")
            (cond
             ((looking-at "^\\$")
              (princ "Variable ")
              (princ (buffer-substring (point)
                                       (progn (search-forward ";")
                                              (forward-char -1)(point))))
              (princ (buffer-substring (progn (forward-char 2)(point))
                                       (progn (end-of-line)(point)))))
             (t
              (princ (gre-format-display-command 
                      (buffer-substring (progn (search-forward ";") (point))
                                        (progn (search-forward ";") (point)))))
              (princ (concat "\n  " (buffer-substring 
                                     (point) (progn (end-of-line)(point)))))))
            (forward-line 1))
          (print-help-return-message)
          (gre-showcase-help-buffer))
      (message "Nothing suitable"))))

(defun gre-help ()
  "Displays help (in *help* buffer) about a prompted gre command.
The help text is taken from gre.cmd (a gre system file) and may differ
from the gre info file (displayed by gre-info).  Help is also displayed 
about user commands from ~/.grerc or from the current gre file.

BUGS:  Can't find help on hidden user commands."
  (interactive)
  (let ((the-command (regexp-quote (gre-prompt-for-command t))))
    (if (string-equal "" the-command)
        (message "No command to look-up.")
      (save-excursion
        (gre-help-function the-command)))))

(defun gre-help-this-command (call-help)
  "Displays help (in *help* buffer) about gre command on point.
The help text is taken from gre.cmd (a gre system file) and may differ
from the gre info file (display by gre-info-this-command).  Help is also
displayed about user commands from ~/.grerc or from the current gre file.

The gre command may span many line, e.g.

  draw x axis \
     at ..ymargin.. \    <gre-help-this-command> 
        {rpn ..xmargin.. .2 -} cm

  will work.

BUGS:  Can't find help on hidden user commands."
  (interactive "P")
  (if call-help 
      (gre-help)
    ;;;FIXME: Here I need to extract the command name like "set x axis"
    ;;;       and provide that as arg to gre-help-function.
    (let ((the-command (gre-isolate-this-validated-command nil)))
      (if (not the-command)
          (error "Sorry, cannot find such a gre command")
        (gre-help-function the-command)))))

(defun gre-help-function (the-command)
  "Actual work for gre-help and gre-help-this-command"
  (let ((gre-tmp-buffer (get-buffer-create "*gre-tmp-buffer*"))
        (syntax-buffer (gre-syntax-buffer 2))
        (user-flag))
    ;; We've gotten this far; valid command.  But gre or user command?
    ;;;FIXME: User command won't be in here...
    (save-excursion
      (set-buffer syntax-buffer)
      (if (re-search-backward (concat "^" the-command ";") nil t)
          (setq user-flag t)))
    (if user-flag
        ;; user-command -- look in current buffer, else in .grerc
        (save-excursion
          (goto-char (point-min))
          (cond 
           ((search-forward (concat "`" the-command "'") nil t) ;in buffer
            (gre-extract-help-text the-command t))
           ((file-readable-p "~/.grerc")
            (set-buffer gre-tmp-buffer) ; look in .grerc
            (insert-file-contents "~/.grerc")
            (if (search-forward (concat "`" the-command "'") nil t)
                (gre-extract-help-text the-command t)
              (kill-buffer gre-tmp-buffer)
              (error "Sorry, can't find user command: %s" the-command)))
           (t
            (kill-buffer gre-tmp-buffer)
            (error "Sorry, can't find user command: %s" the-command))))
      ;; gre system command
      (set-buffer gre-tmp-buffer)
      (insert-file-contents (gre-file-path t))
      (when (not (re-search-forward (concat "^\\(#\\* @\\)?cmd " 
                                          the-command " (") nil t))
        (kill-buffer gre-tmp-buffer)
        (error "Sorry, can't find help about: %s" the-command))
      (gre-extract-help-text the-command nil))
    (kill-buffer gre-tmp-buffer)
    ))


(defun gre-extract-help-text (the-command user-flag)
  "Extract help text after a command.  Expecting point on title line.
Display message if no help text supplied."
  (re-search-forward "# *\\(.*\\)" nil t)
  (let ((the-syntax (match-string 1)))
    (forward-line -1)
    (if (not (looking-at "^#\\*"))
        (if user-flag
            (message "Sorry, no help text for user command: %s" the-command)
          (message "Sorry, no help text about: %s" the-command))
      (while (and (looking-at "^#\\*")
                  (equal 0 (forward-line -1))))
      (if (not (looking-at "^#\\*"))
          (forward-line 1))
      (let ((the-text))
        (while (looking-at "^#\\*")
          (skip-chars-forward "#* ")
          (setq the-text 
                (concat the-text 
                        "\n" 
                        (buffer-substring (point) 
                                          (progn (end-of-line)(point)))))
          (forward-line 1))
        (with-output-to-temp-buffer " *Gre Help*"
          (princ (gre-format-display-command 
                  (concat the-command " " the-syntax)))
          (princ "\n--\n")
          (princ the-text)))
      (gre-showcase-help-buffer))))

(defun gre-showcase-help-buffer ()
  "Make the *Gre Help* buffer pretty"
;;;FIXME: I could make a small mode here and add hypertext links.
  (save-excursion
    (set-buffer " *Gre Help*")
    (make-local-variable 'font-lock-defaults)
    (setq font-lock-defaults
          '(gre-help-font-lock-keywords
            t             ;;; Keywords only? No, let it do syntax via table.
            nil           ;;; case-fold?
            nil           ;;; Local syntax table.
            nil           ;;; Use `backward-paragraph' ? No
            ;;(font-lock-comment-start-regexp . "#")
            ;;(font-lock-mark-block-function . mark-paragraph)))
            ))
    (if (featurep 'font-lock)
        (font-lock-fontify-buffer))))

(defun gre-format-display-command (the-command)
  "return possible 2-line string for ARG, a gre command syntax string."
  (if (> (screen-width) (length the-command))
      the-command
    (let ((the-string)
          (tmp-buffer (get-buffer-create "*gre-format*")))
      (save-excursion
        (set-buffer tmp-buffer)
        (insert the-command)
        (backward-char 1)
        (if (or (char-equal 93  (char-after (point)))   ;; []
                (char-equal 125 (char-after (point))))  ;; {}
            (progn
              (forward-char 1)
              (goto-char (scan-lists (point) -1 0))
              (insert "\n    "))
          ;; just split at previous whitespace
          (if (search-backward " " nil t)
              (insert "\n    ")))
        (setq the-string (buffer-string))
        (kill-buffer tmp-buffer))
      the-string)))

(defun gre-shorten-guess-command (guess separator)
  "Removes a word (separated by ARG2) from end of ARG1 
as a new guess to a gre command"
  (let ((lastindex nil) (index (string-match separator guess nil)))
    (if (not index)
        nil
      (while index
        (setq lastindex index)
        (setq index (string-match separator guess (1+ index))))
      (substring guess 0 lastindex))))

(defun gre-WWW-manual ()
  "Start world-wide-web browser displaying gre manual.
The page visited is set in the variable gre-WWW-page, which may be reset on 
your site.  The main site (always up to date) is:

 http://www.phys.ocean.dal.ca/~kelley/gre/gre1.html

The browser used by determined by the variable gre*WWW-program.
Any output (errors?) is put in the buffer `gre-WWW-manual'."
  (interactive)
  (cond 
   (gre*WWW-program
    (message "Starting process. See buffer gre-WWW-manual about errors.")
    (setq gre*WWW-process
          (start-process "gre-WWW" "gre-WWW-manual" gre*WWW-program
                         gre*WWW-page)))
   (t
    (if (not (featurep 'browse-url))
        (load "browse-url" t t))
    (if (not (featurep 'browse-url))
        (message "Sorry, you don't have browse-url on your system.  Set variable gre*WWW-program")
      (funcall browse-url-browser-function gre*WWW-page)))))

;;;----------
;;; Info interface functions.

(defun gre-info ()
  "Runs info about a prompted gre system command."
  (interactive)
  (require 'info)
  (let ((the-command (gre-prompt-for-command nil)))
    (if (string-equal "" the-command)
        (message "No command to look-up.")
      (gre-info-function the-command))))

(defun gre-info-this-command (call-info)
  "Run info about gre system command at editing point.  
This works when the edit point is on a valid gre command, but it may also
work for uncompleted commands if an info entry exists for that topic.
e.g. 

  draw <gre-info-about-line>

 will display info about gre `draw' commands.

Note: The search for the command is case-insensitive.  Therefore, if
you have a user command like `Draw X Axis', gre-info-this-command will
display the info page for the gre command `draw x axis'."
  (interactive "P")
  (require 'info)
  (if call-info 
      (gre-info)
    (gre-info-function (gre-isolate-this-command))))

(defun gre-info-function (guess)
  "Guts for gre-info and gre-info-this-command"
  (if (not (gre-info-directory))
      (error "Sorry, no gre info files!"))
  (let ((flag t)(loopflag t)(tmp-buffer (get-buffer-create "*info-tmp*")))
      (save-excursion
        (set-buffer tmp-buffer)
        (cond
         ((fboundp 'info-insert-file-contents) ;Emacs20
          (info-insert-file-contents (concat (gre-info-directory) "gre")))
         ((and (fboundp 'Info-insert-file-contents) ;XEmacs20
              (fboundp 'Info-suffixed-file))
          (Info-insert-file-contents 
           (Info-suffixed-file (concat (gre-info-directory) "gre")) nil))
         (t                             ; old emacs?
          (insert-file-contents (concat (gre-info-directory) "gre"))))
        (goto-char (point-min))
        (if (re-search-forward 
             (concat "Node: " (regexp-quote guess) "\177") nil t)
            (setq flag t)
          (while loopflag  ;; flag true until can't shorten or found
            (setq guess (gre-shorten-guess-command guess " "))
            (if guess 
                (if (re-search-forward (concat "Node: " (regexp-quote guess) 
                                               "\177") nil t)
                    (setq loopflag nil)) ;; found it!
              (setq loopflag nil)  ;; can't shorten no more
              (setq flag nil)))))  ;; failed to find a match
      (kill-buffer "*info-tmp*")
      (if flag
          ;;??Replace with: (Info-find-node "gre" guess)
          (Info-goto-node (concat "(gre)" guess))
        (message "Sorry, can't find this topic in Info."))))

(defun gre-info-directory ()
  "Returns nil or gre info file path 
Path is from Info-directory-list and gre info file may be compressed, with 
suffix .Z, .z or .gz"
  (require 'info)
  (if (not (boundp 'Info-directory-list))
      nil
    (let ((work-list Info-directory-list)
          (notfound t)
          (info-directory nil))
      (while (and notfound (car work-list))
        (if (or (file-readable-p (concat (car work-list) "gre"))
                (file-readable-p (concat (car work-list) "gre.Z"))
                (file-readable-p (concat (car work-list) "gre.z"))
                (file-readable-p (concat (car work-list) "gre.gz")))
            (progn
              (setq info-directory (car work-list))
              (setq notfound nil))
          (setq work-list (cdr work-list))))
      info-directory)))

(defun gre-narrow-to-function ()
  "Restrict editing in this buffer to the current gre function.
The rest of the text becomes temporarily invisible and untouchable
but is not deleted; if you save the buffer in a file, the invisible
text is included in the file.  C-x n w makes all visible again."
  (interactive)
  (save-excursion
    (let ((the-begin))
      (if (not (re-search-backward "^\\(cmd\\|sub\\).*$"nil t))
          (message "Sorry, can't find the beginning of the function.")
        (setq the-begin (point))
        (if (not (re-search-forward "^}$" nil t))
            (message "Sorry, can't find the end of the function to.")
          (narrow-to-region the-begin (point)))))))

(defun gre-comment-out-region (from to)
  "Comment out the region between point and mark.
You can remove these comments using gre-uncomment-out-region."
  (interactive "r")
  (save-excursion
    (save-restriction
      (narrow-to-region from to)
      (goto-char (point-min))
      (let ((overwrite-mode nil))
        (insert "# ")
        (while (and (= 0 (forward-line 1))
                    (not (= (point) (point-max))))
          (insert "# "))))))
 
(defun gre-uncomment-out-region (from to)
  "Remove Comments at beginning of lines in the region between point and mark."
  (interactive "r")
  (save-excursion
    (save-restriction
      (narrow-to-region from to)
      (goto-char (point-min))
      (if (looking-at "# ")
          (delete-char 2)
        (if (looking-at "#")
            (delete-char 1)))
      (while (= 0 (forward-line 1))
      (if (looking-at "# ")
          (delete-char 2)
        (if (looking-at "#")
            (delete-char 1)))))))

(fset 'gre-indent-line 'perl-indent-line)

;; I must redefine `perl-beginning-of-function' to recognize the gre `cmd'
;; keyword to define a new command.
(defun perl-beginning-of-function (&optional arg)
  "Move backward to next beginning-of-function, or as far as possible.
With argument, repeat that many times; negative args move forward.
Returns new value of point in all cases."
  (interactive "p")
  (or arg (setq arg 1))
  (if (< arg 0) (forward-char 1))
  (let ((func-regexp 
         (if (string-equal mode-name "gre")
             "^\\s(\\|^\\s-*\\(cmd\\|sub\\)\\b[^{]+{\\|^\\s-*format\\b[^=]*=\\|^\\."
           "^\\s(\\|^\\s-*sub\\b[^{]+{\\|^\\s-*format\\b[^=]*=\\|^\\.")))
    (and (/= arg 0)
         (re-search-backward func-regexp nil 'move arg)
         (goto-char (1- (match-end 0))))
    (point)))

(defun gre-indent-buffer ()
  "Indent the entire gre buffer. Won't indent hidden user commands."
  (interactive)
  (save-excursion
    (goto-char (point-min))
    (skip-chars-forward "\n")
    (gre-indent-line)
    (while (= 0 (forward-line 1))
      (gre-indent-line))))

(defun gre-indent-region (from to &optional entire-buffer-flag)
  "Indent the region.  With Prefix argument, indent entire buffer."
  (interactive "r\nP")
  (save-excursion
    (if entire-buffer-flag
        (gre-indent-buffer)
      (save-restriction
        (narrow-to-region from to)
        (goto-char (point-min))
        (skip-chars-forward "\n")
        (narrow-to-region (point) (point-max))
        (gre-indent-line)
        (while (= 0 (forward-line 1))
          (gre-indent-line)))))
  (gre-indent-line))
  
(defun gre-insert-file-as-comment (&optional number-of-lines)
  "Insert one line as a gre comment, taking filename from under point.
Optional number of lines may be given as prefix argument.

This is useful to comment an `open' statement, e.g. with editing point 
on the filename, the following line:
 open gps.dat
gets the following comment:
 open gps.dat
 //  80 31.224'N  11 48.633'W 10:03:18.2 07/05/93"
  (interactive "p")
  (let ((name (gre-file-name-at-point))
        (the-end))
    (if (not name)
        (message "Sorry, could not detect file name under editing point")
      (save-excursion
        (end-of-line)
        (newline)
        (shell-command (concat "head -"(int-to-string number-of-lines) 
                               " " name) t)
        (while (< 0 number-of-lines)
          (gre-indent-line)
          (insert "// ")
          (forward-line 1)
          (setq number-of-lines (1- number-of-lines)))
        (setq the-end (point)))
      (if (and gre*use-hilit19 (fboundp 'hilit-rehighlight-region))
        (hilit-rehighlight-region (point) the-end t)))))

(defun gre-string-at-point (chars &optional punct)
  "Return maximal string around point, of chars specified by string CHARS.
Chars from the optional PUNCT string are stripped from the end."
  (buffer-substring 
   (save-excursion (skip-chars-backward chars) (point))
   (save-excursion 
     (let ((pt (point)))
       (skip-chars-forward chars)
       ;; skip back over punctuation, but not beyond pt:
       (and punct (skip-chars-backward punct pt))
       (point)))))
  
(defun gre-file-name-at-point ()
  "Return filename from around point if it exists, or nil.
Based on ffap.el from: mic@cs.ucsd.edu (Michelangelo Grigni)"
  (let* ((case-fold-search t)
	 (name (gre-string-at-point "--:$+<>@-Z_a-z~" ";.,!?")))
      (cond
       ((zerop (length name)) nil)
       ((file-exists-p name) name))))

;;;----------
;;; Command Completion

(defun gre-complete ()
  "Complete a gre command as much as possible, then indents it.
   Works one word at a time,
    e.g.  
        dr<complete>
      (where <complete> is actually pressing M-Tab) becomes
        draw
    or with many words at a time
     e.g. 
        dr x a
      becomes
        draw x axis [at bottom|top|{$y [cm]} [lower|upper]]

Possible Completions:

   Possible completions are shown in the *completions* buffer,
   but only if you insist by using gre-complete again.  In this
   way you can use gre-complete word-by-word to abbreviate commands
   without ever displaying completions, like you would for file 
   completion in emacs or bash.

   If a completion is ambiguous, but could be exact, invoke
   gre-complete a second time to complete it.
     e.g.
        sh<complete>
      becomes
        show
      and informs you that 12 possible completions exists;
      then
        show<complete>
      will display these completions in the completions buffer;
      then
        show<complete>
      forces completion to a complete but not unique possibility. 
        show .value.|{rpn ...}|\"\\text\" [.value.|{rpn ...}|text [...]]

   Completions are shown immediately (without invoking gre-complete 
   again) if the completions window is already displayed or if there 
   are 3 possbilities or less.  In this case they are displayed in 
   the minibuffer. 
  
   The *completions* window is deleted after a command is fully completed.
   gre-complete uses its own *completions* buffer, which is not displayed
   in the buffer-list to avoid clutter.

User Commands:

   User gre commands defined in ~/.grerc or within the gre file currently 
   being edited can also be gre-complete'd.

Gre Code Fragments

   gre has special commands that begin with a question mark `?'.  These
   special commands have no options, and are composed only of standard gre
   commands.  Their purpose is to provide a short-cut for entering many lines
   of gre at once (e.g. bits of sample code about contouring grids, or your
   own preamble which you use at the time to set fonts and line widths).

   gre-complete acts in a special way with these commands, by replacing
   the abbreviated name which you are completing by all the lines 
   contained within the gre command.

   The user is allowed to define new fragments in ~/.grerc.
   To see the names of all gre fragments, type in a question mark at the 
   beginning of a line in a gre buffer and press M-Tab twice to gre-complete 
   it and display possible choices.
-
Bugs:
   Completions relies on entire command, not just up to the editing point."
;;Sets gre-last-complete-point   to point after completion (if matches > 1)
;;Sets gre-last-complete-command to current line (if matches > 1)
  (interactive)
  (save-restriction
    (let ((the-begin (save-excursion
                       (re-search-backward 
                        "[;\\$]" 
                        (save-excursion (beginning-of-line) (point))
                        1)
                       (point))))
      (narrow-to-region the-begin (point)) 
      (let ((expansion-regex (gre-build-expansion-regex))
            (the-line (buffer-substring-no-properties 
                       gre-complete-begin-point (point-max))))
        (if (eq expansion-regex nil)
            (progn
              (message "No gre command no expand.")
              (setq gre-last-complete-status 0))
          (if (and (= 2 gre-last-complete-status) ; try to match exactly
                   (= (point) gre-last-complete-point)
                   (string-equal the-line gre-last-complete-command))
              (gre-perform-completion (concat expansion-regex ";") t)
            (if (and (= 1 gre-last-complete-status) ; show completions
                     (string-equal the-line gre-last-complete-command))
                (gre-perform-completion expansion-regex nil)
              (setq gre-last-complete-status 0)
              (gre-perform-completion expansion-regex nil)))
          (if (> gre-last-complete-status 0)
              (save-excursion
                (setq gre-last-complete-point (point))
                (setq gre-last-complete-command 
                      (buffer-substring 
                       (point)
                       (progn (if (search-backward 
                                   ";" 
                                   (save-excursion 
                                     (beginning-of-line) (point)) 1)
                                  (forward-char 1))
                              (point)))))))))))

(defun gre-perform-completion (expansion-regex exact-flag)
  "Does the actual completion based on EXPANSION-REGEX. 
Returns number of matches.
Second argument EXACT-FLAG is t if trying for exact match.  
If we fail we will display a different message.
Sets gre-last-complete-status to 1 if show completions next time
                              to 2 if expand complete match next time
                               (used by gre-complete only, not here)
                              to 0 in other cases."
  (let ((unique nil) (match-count 0) (expansion-list nil))
    ;;current buffer commands and variables
    (save-excursion
      (save-restriction
        (widen)
        (goto-char 1)
        (cond
         ((string-match "\\^\\\\\\$" expansion-regex)
          (let ((regexp (concat "^[ \t]*\\("
                                (substring expansion-regex 1)
                                ".*\\)[ +-/*]+=")))
            (while (re-search-forward regexp nil t)
              (setq expansion-list (cons (buffer-substring-no-properties 
                                          (match-beginning 1)(match-end 1))
                                         expansion-list))
              (forward-line 1)
              (setq match-count (1+ match-count)))))
         (t
          (let ((regexp (concat "^\\(cmd\\|sub\\) \\("
                                (substring expansion-regex 1)
                                "\\)")))
            (while (re-search-forward regexp nil t)
              (setq expansion-list 
                    (cons (buffer-substring-no-properties 
                           (match-beginning 2)
                           (progn (search-forward "(")
                                  (forward-char -1)
                                  (skip-chars-backward " ")(point)))
                          expansion-list))
              (forward-line 1)
              (setq match-count (1+ match-count))))))))
    ;;system commands and variables
    (save-excursion
      (set-buffer (gre-syntax-buffer 0))
      (while (re-search-forward expansion-regex nil t)
        (setq expansion-list (cons (buffer-substring
                                    (progn (beginning-of-line) (point)) 
                                    (progn (search-forward ";")
                                           (backward-char 1)(point)))
                                   expansion-list))
        (forward-line 1)
        (setq match-count (1+ match-count))))
    (cond
     ((= match-count 0)
      (if exact-flag
          (message "Sorry, this does not match *exactly* to any gre command.")
        (message "Sorry, this does not match to any known gre command."))
      (setq gre-last-complete-status 0))
     (t
      (delete-region gre-complete-begin-point (point))
      (cond
       ((= match-count 1)
          ;; FIXME: Do we still support fragments?
;;        (save-excursion
;;          (if (string-equal "?" (substring (car expansion-list) 0 1))
;;              (progn           
;;                (set-buffer (gre-syntax-buffer 0))
;;                (re-search-forward (concat "^" (regexp-quote 
;;                                                (car expansion-list))) 
;;                                   nil t)
;;                (forward-line 1)
;;                (setq unique
;;                      (buffer-substring (point)
;;                                        (progn
;;                                          (re-search-forward "^[^ \t]" nil t)
;;                                          (backward-char 1)(point)))))
        (save-excursion
          (set-buffer (gre-syntax-buffer 0))
          (cond 
           ((re-search-forward (concat "^" (regexp-quote
                                            (car expansion-list)))
                               nil t)
            (forward-char 1)
            (cond
             ((string-equal "$" (substring (car expansion-list) 0 1))
              (setq unique (buffer-substring
                            (progn (beginning-of-line) (point))
                            (progn (search-forward ";") (forward-char -1)
                                   (point))))
              ;;Display message with default setting for this variable
              (message "%s" (buffer-substring
                             (progn (forward-char 2)(point))
                             (progn (end-of-line) (point)))))
             (t
              (setq unique (buffer-substring 
                            (point)
                            (progn (search-forward ";")(point))))
              (if (looking-at ".*(\\(default is .*\\))$")
                  (message (match-string 1))))))))
        ;; If not set yet, then the command is local to current buffer
        (if (not unique)
            (save-excursion
              (goto-char (point-min))
              (cond
               ((string-equal "$" (substring (car expansion-list) 0 1))
                (setq unique (car expansion-list)))
               (t
                (if (re-search-forward (concat "^\\(cmd\\|sub\\) " 
                                               (regexp-quote
                                                (car expansion-list))
                                               "[ ]*(") nil t)
                    (if (re-search-forward "#[ ]*\\(.*\\)" 
                                           (save-excursion
                                             (end-of-line)(point))
                                           t)
                        (setq unique (concat (car expansion-list) " "
                                             (match-string 1) ";"))
                      (setq unique (concat (car expansion-list) ";"))))))))
        ;; FIXME: this can be simplied if fragments are removed
        (let ((the-start (point))         ; indent all inserted lines
              (the-end (progn (insert unique) (perl-indent-line) 
                              (point-marker))))
          (goto-char the-start)
          (perl-indent-line)
          (while (and (= 0 (forward-line 1))
                      (< (point) (marker-position the-end)))
            (perl-indent-line))
          (goto-char (marker-position the-end)))
        (if (and (get-buffer " *Completions*")  ;;need line for emacs-18
                 (get-buffer-window (get-buffer " *Completions*")))
            (delete-window (get-buffer-window (get-buffer " *Completions*")))
          (if (get-buffer-window (get-buffer " *Completions*") 'visible)
              (iconify-frame 
               (window-frame 
                (get-buffer-window (get-buffer " *Completions*") 'visible)))))
        (setq gre-last-complete-status 0))
       ((or (= 1 gre-last-complete-status) ;display completions 
            (and (get-buffer " *Completions*")
                 (get-buffer-window (get-buffer " *Completions*") 'visible))) 
        (insert (gre-common-in-list expansion-list))
        (perl-indent-line)
        (message "%d possible completions" match-count) 
        (with-output-to-temp-buffer " *Completions*"
          (display-completion-list expansion-list))
        (setq gre-last-complete-status 2)) ;Next time, try unique match
       ((< match-count 4)               ; 3 or fewer matches show in minibuffer
        (insert (gre-common-in-list expansion-list)) 
        (perl-indent-line)
        (message "(%d) %s" match-count (mapconcat 'format expansion-list "  "))
        (setq gre-last-complete-status 2))
       (t                                 ;complete as much as possible
        (insert (gre-common-in-list expansion-list)) 
        (perl-indent-line)
        (message "%d possible completions" match-count) 
        (setq gre-last-complete-status 1))))) ;show completions next time
    match-count))

(defadvice choose-completion-string (after gre-mode-choose-completion protect activate)
  "Perform completion on mouse selected text."
  (when (string-equal major-mode "gre-mode")
    (delete-backward-char 1)
    (gre-complete)))

(defun gre-build-expansion-regex ()
  "Returns regular expression for abbreviated gre command on current line."
;; Assumes region is narrowed to text of interest
  (let ((expansion-regex nil) 
        (word-count 0)
        (end-point (point)))
    (save-excursion
      (if (and (re-search-backward 
                "[;\\$]" (save-excursion (beginning-of-line) (point)) 1)
               (looking-at "\\$[^ \n;]+"))
          (progn 
            (setq gre-complete-begin-point (point))
            (concat "^" (regexp-quote (match-string 0))))
        (if (looking-at ";")
            (forward-char 1))
        (skip-chars-forward " \t")
        (setq gre-complete-begin-point (point))
        (while (re-search-forward "[^ \t]+" end-point t)
          (setq word-count (1+ word-count))
          (if (= word-count 1)
              (setq expansion-regex 
                    (concat "^" (regexp-quote (match-string 0))))
            (setq expansion-regex 
                  (concat expansion-regex "[^ ;]* " 
                          (regexp-quote (match-string 0)))))
          (skip-chars-forward " \t"))
        expansion-regex))))

(defun gre-common-in-list (list)
  "returns STRING with same beginnings in all strings in LIST"
;;;FIXME: very similar to `(try-completion "" alist)'
;;;       -> (try-completion "" '(("atest") ("atry")))
;;;          returns "at"
  (let ((i 1)
        (work-list list) 
        (match-len (length (car list))) 
        (first-string (car list))
        (current-string nil)
        (match-flag t))
    (setq work-list (cdr work-list))
    (while work-list
      (setq current-string (car work-list))
      (if (< (length current-string) match-len)
          (setq match-len (length current-string)))
      (when current-string ;; nil last time around
        (while (and (<= i match-len) match-flag)
          (if (equal (substring first-string 0 i) 
                     (substring current-string 0 i))
              (setq i (1+ i))
            (setq match-flag nil)))
        (setq match-len (1- i))
        (setq match-flag t)
        (setq i 1))
      (setq work-list (cdr work-list)))
    (substring first-string 0 match-len)))

;;;----------
;;; Editing functions.

(defun gre-kill-option-mouse (EVENT)
  "Kill (delete) a gre command option, variable or string.
See  `C-h f gre-kill-option' for more help"
  (interactive "e")
  (mouse-set-point EVENT)
  (gre-kill-option))

(defun gre-kill-option ()
  "Kill (delete) a gre command option, variable or string.
An option is everything within brackets.  This has highest priority.
e.g. after command completion, you get something like:

  draw x axis [at bottom|top|{$y [cm]} [lower|upper]]
   put the cursor here and C-c C-k ^ yields:
  draw x axis [at bottom|top|{$y } [lower|upper]]
       then here ^ and press C-c C-k
  draw x axis
       because it delete the outermost bracketed option.
  
If the cursor is not within a bracketed option, and is within
a string or on the first character or a variable, then that string
or variable is deleted."
  (interactive)
  (save-restriction
    (gre-narrow-to-command-on-one-line)
    (let ((here-point (save-excursion (forward-char 1)(point)))
          (del-flag nil) (brk-point))
      (save-excursion                     ;Start with bracket options
        (while (and (not del-flag) (re-search-backward "[{[]" (point-min) t))
          (setq brk-point (point))
          (save-excursion
            (goto-char (scan-lists (point) 1 0))
            (when (>= (point) here-point)
              (setq del-flag t)
              (delete-region (point) brk-point)
              (delete-horizontal-space)
              (insert " ")))))
      (if del-flag
          
          (gre-next-option)               ; Visit next option
        ;; Check if on first char of a variable, delete it.
        (if (string-match "^$[A-z0-1]+" 
                          (buffer-substring (point)
                                            (progn (save-excursion
                                                     (end-of-line)(point)))))
            (delete-char (match-end 0))
          ;; Check if within a string, delete it
          (save-excursion
            (if (and (re-search-backward "\"" (point-min) t) 
                     (progn 
                       (forward-char 1)
                       (setq brk-point (point))
                       (re-search-forward 
                        "\"" (progn (save-excursion (end-of-line) (point))) t))
                     (>= (point) here-point))
                (delete-region 
                 (progn (forward-char -1)(point)) brk-point))))))))

(defun gre-next-option ()
  "Go to next option on line (options first, variables and strings second)"
  (interactive)
  (let ((end-line (save-excursion (end-of-line)(point)))
        (the-point (point)))
      ;; first do brackets, then variables or strings
    (or (re-search-forward "[{[|]" end-line t)
        (progn (beginning-of-line) (re-search-forward "[{[]" end-line t))
        (if (or (progn 
                  (goto-char the-point) 
                  (re-search-forward " [\"$]" end-line t))
                (progn
                  (beginning-of-line) 
                  (re-search-forward " [\"$]" end-line t)))
            ;;move backward if on a variable
            (if (equal 36 (char-after (point)))
                (forward-char -1))))))

(defun gre-option-select-mouse (EVENT)
  "Select a gre option left by gre-complete. See gre-option-select for help."
  (interactive "e")
  (mouse-set-point EVENT)
  (gre-option-select))

(defun gre-option-select ()
  "Select a gre option left by gre-complete.

  Narrow in on a particular gre command, given a syntax description left
on the line by gre-complete.  The cursor location is used to decide
which gre command(s) to narrow to.

EXAMPLE: If gre-complete is used on the line `dr x a', the result will
be a line like
  draw x axis [at bottom|top|{.y. [cm]} [lower|upper]]

This is the Gre way of describing many commands at once.  All these
commands are described by this syntax description:
  draw x axis
  draw x axis at bottom
  draw x axis at bottom top
  draw x axis at bottom bottom
  draw x axis at top
  draw x axis at top top
  draw x axis at top bottom
  draw x axis at .y. cm
  draw x axis at .y. cm lower
  draw x axis at .y. cm upper

The gre-option-select command provides easy navigation to select one
of these commands.  The narrowing process is governed by the cursor
position.  For example, to get the command narrowed down to
  draw x axis at bottom [lower|upper] place the cursor somewhere in
the word `bottom' and invoke gre-option-select.  To complete the
narrowing process, selecting
  draw x axis at bottom lower
move the cursor to some place in the word `lower' and invoke
gre-option-select again.  On the other hand, to get
  draw x axis at bottom
you would put the cursor over either the word `lower' or `upper', and
invoke gre-kill-option.

NOTE: you might want to practice using this example to learn how to do
it.  If you make a mistake, note that the normal Emacs undo works."
  (interactive)
;;Algorithm is  1) remove other options within brackets
;;              2) while there are grouping brackets
;;                   remove options on either side of brackets
;;                   remove brackets
  (save-restriction
    (gre-narrow-to-command-on-one-line)
    (let ((here-point (save-excursion (forward-char 1)(point))))
      (save-excursion
        (skip-chars-backward "^ |[]{}")
        (gre-del-group-opt-backward))
      (save-excursion
        (skip-chars-forward "^ |[][}")
        (gre-del-group-opt-forward))
      (save-excursion
        (while (< (point-min) (gre-return-enclosing-group nil (point-min)))
          (goto-char (gre-return-enclosing-group nil (point-min)))
          ;; on a start bracket of a group
          (save-excursion
            (goto-char (scan-lists (progn (backward-char 1)(point)) 1 0))
            (delete-backward-char 1)    ;; delete the closing bracket
            (gre-del-group-opt-forward))
          (delete-char 1)       ;; delete the opening bracket
          (gre-del-group-opt-backward)
          (setq here-point (point))))
      (gre-next-option))))
    
(defun gre-narrow-to-command-on-one-line ()
  (narrow-to-region
   (save-excursion 
     (if (search-backward ";" (save-excursion (beginning-of-line)(point)) 1)
         (forward-char 1))
     (point))
   (save-excursion 
     (if (search-forward ";" (save-excursion (end-of-line)(point)) 1)
         (forward-char -1))
     (point))))

(defun gre-del-group-opt-forward ()
  "Called by gre-option-select to delete backward options within brackets
     e.g.   [arg1|arg2]|[[ar3 arg4]|arg5|arg6]|arg8
                        |               ^    |
         becomes
            [arg1|arg2]|[arg5|ar6]|arg8
Always starts on first character to delete."
  (save-excursion
    (let ((end-point (gre-return-enclosing-group 
                      t (progn (save-excursion (end-of-line) (point)))))
          (here-point (point)))
      ;; see if followed by |, if so select option after it for deletion
      (while (and (progn (skip-chars-forward " \t")(looking-at "|"))
                  (> end-point (point)))           ;; and still within group
        (forward-char 1)(skip-chars-forward " \t")
        (if (looking-at "[{[]")                    ;; on an opening bracket
            (goto-char (scan-lists (point) 1 0))
          ;; on a simple word--find end
          (if (re-search-forward "[] |{}[]" end-point t)
              (backward-char 1)                   ;; don't delete bracket or sp
            (goto-char end-point))))
      (skip-chars-backward "; \t")
      (if (< end-point (point))
          (goto-char end-point))
      (delete-region (point) here-point))))

(defun gre-del-group-opt-backward ()
  "Called by gre-option-select to delete backward options within brackets
     e.g.   [arg1|arg2]|[[ar3 arg4]| arg5|arg6]|arg8
                        |           ^        |
         becomes
            [arg1|arg2]|[arg5|ar6]|arg8
Always starts on first character to keep."
  (save-excursion
    (let ((beg-point (gre-return-enclosing-group 
                      nil (progn (save-excursion (beginning-of-line)(point)))))
          (here-point (point)))
      ;; see if preceeded by |, if so select option before it for deletion
      (while (progn (save-excursion 
                      (and (progn (skip-chars-backward " \t")(backward-char 1)
                                  (looking-at "|"))   ;; preceeded by | 
                           (< beg-point (point)))))   ;; and still within group
        (skip-chars-backward " \t")(backward-char 1)  ;; on the |
        (skip-chars-backward " \t")(backward-char 1)  ;; on the previous word
        (if (looking-at "}\\|]")                      ;; on a closing bracket 
            (progn (forward-char 1)(goto-char (scan-lists (point) -1 0)))
          (if (re-search-backward "[] |{}[]" beg-point t)  ;; on a simple word
              (forward-char 1)                             ;; on first char
            (goto-char beg-point)(forward-char 1))))
      (skip-chars-forward " \t")
      (when (> beg-point (point))
          (goto-char beg-point) (forward-char 1))
      (delete-region (point) here-point))))
    
(defun gre-return-enclosing-group (end_flag boundary)
  "Return as a point either the beginning (ARG1 nil) or end (ARG1 t)
of the first enclosing group, either curly or square brakets.
  [   ]
  ^   ^ 
The boundaries are the points not to be exceeded (e.g. end-of-line
for ARG1 t, beginning-of-line for ARG1 nil).
Return that boundary if no containing group within that boundary."
  (save-excursion
    (let ((the-point) (here-point (point)))
      (if end_flag
          (progn
            (while (and (re-search-forward "[]}]" boundary t)
                        (< here-point ;; while group doesn't enclose cursor
                           (progn 
                             (save-excursion 
                               (goto-char (scan-lists (point) -1 0))
                               (backward-char 1)
                               (setq the-point (point))
                               the-point)))))
            ;; return either end of group or boundary
            (if (and the-point                  ;; found at least one group
                     (> here-point the-point))  ;; and it encloses us
                (point)                         ;; then return end of group
              boundary))                        ;; else return boundary  
        ;; end_flag is nil
        (while (and (re-search-backward "[{[]" boundary t)
                    (> here-point ;; while group doesn't enclose cursor
                       (progn 
                         (save-excursion 
                           (goto-char (scan-lists (point) 1 0))
                           (setq the-point (point))
                           the-point)))))
        ;; return either beginning of group or boundary
        (if (and the-point                  ;; found at least one group
                 (< here-point the-point))  ;; and it encloses us
            (point)                         ;; then return end of group
          boundary)))))                     ;; else return boundary  

;;; -------------------
;;; Inserting text and other edits

(defun gre-narrow-to-function ()
  "Restrict editing in this buffer to the current gre function.
The rest of the text becomes temporarily invisible and untouchable
but is not deleted; if you save the buffer in a file, the invisible
text is included in the file.  C-x n w makes all visible again."
  (interactive)
  (save-excursion
    (let ((the-begin))
      (if (not (re-search-backward "^`.*'$" nil t))
          (message "Sorry, can't find the beginning of the function.")
        (setq the-begin (point))
        (if (or (not (re-search-forward "^{$" nil t))
                (not (re-search-forward "^}$" nil t)))
            (message "Sorry, can't find the end of the function to.")
          (narrow-to-region the-begin (point)))))))

(defun gre-insert-file-as-comment (&optional number-of-lines)
  "Insert one line as a gre comment, taking filename from under point.
Optional number of lines may be given as prefix argument.

This is useful to comment an `open' statement, e.g. with editing point 
on the filename, the following line:
 open gps.dat
gets the following comment:
 open gps.dat
 //  80 31.224'N  11 48.633'W 10:03:18.2 07/05/93"
  (interactive "p")
  (let ((name (gre-file-name-at-point))
        (the-end))
    (if (not name)
        (message "Sorry, could not detect file name under editing point")
      (save-excursion
        (end-of-line)
        (newline)
        (shell-command (concat "head -"(int-to-string number-of-lines) 
                               " " name) t)
        (while (< 0 number-of-lines)
          (perl-indent-line)
          (insert "# ")
          (forward-line 1)
          (setq number-of-lines (1- number-of-lines)))))))

(defun gre-string-at-point (chars &optional punct)
  "Return maximal string around point, of chars specified by string CHARS.
Chars from the optional PUNCT string are stripped from the end."
  (buffer-substring 
   (save-excursion (skip-chars-backward chars) (point))
   (save-excursion 
     (let ((pt (point)))
       (skip-chars-forward chars)
       ;; skip back over punctuation, but not beyond pt:
       (and punct (skip-chars-backward punct pt))
       (point)))))
  
(defun gre-file-name-at-point ()
  "Return filename from around point if it exists, or nil.
Based on ffap.el from: mic@cs.ucsd.edu (Michelangelo Grigni)"
  (let* ((case-fold-search t)
	 (name (gre-string-at-point "--:$+<>@-Z_a-z~" ";.,!?")))
      (cond
       ((zerop (length name)) nil)
       ((file-exists-p name) name))))

(defun gre-function-skeleton ()
  "Build a routine skeleton prompting for name.
Enter something like: 
  My Command Name $arg1 $arg2"
  (interactive)
  (let ((the-name (read-string "gre function name: "))
        (the-user (user-full-name)))
    (if (string-equal the-name "")
        (error "No name given."))
    (goto-char (point-min))
    (insert "\n") (backward-char 1)
    (gre*date-function) 
    (insert "\n#*\n")
    (insert "cmd " the-name )
;;; FIXME: I have to handle [options] and keywords here. 
    (let ((limit (save-excursion (beginning-of-line)(point)))
          (count "")
          (args))
      (while (re-search-backward "$[^$ ]+" limit t)
        (setq count (concat "$" count))
        (setq args (cons (match-string 0) args)))
      (if (not args) 
          (insert " () {")
        (insert "(" count ") {  # ")
        (indent-for-comment)
        (end-of-line))
      (insert "\n\n}")
      (forward-line -3)
      (end-of-line)
      (while args
        (insert "\n#* @param " (car args) " ")
        (setq args (cdr args))))
    (cond 
     ((featurep 'font-lock)
      (forward-line 3)
      (end-of-line)
      (save-excursion
        (font-lock-fontify-region (point-min)(point)))
      (forward-line -1))
     (t
      (forward-line 2)))))

;;;----------
;;; Outlining stuff
;; Fixme: Delete this section?

(defun gre-show-function (&optional show-all)
  "Uncover function on current line, hidden by gre-hide commands.
If prefixed, show all functions in current buffer (this may easier than
typing in C-c M-S)."
  (interactive "P")
  (let ((modified (buffer-modified-p)))
    (if show-all 
        (gre-show-all)
      (show-entry)
      (set-buffer-modified-p modified))))

(defun gre-show-all ()
  "Uncover all functions in current buffer, hidden by gre-hide commands."
  (interactive)
  (let ((modified (buffer-modified-p)))
    (show-all)
    (set-buffer-modified-p modified)))

(defun gre-hide-function (&optional hide-all)
  "Hide function under point, similarly to outline-mode.
If prefixed, hide all functions in current buffer.

BUGS:  Will get confused if you have a string which looks like a function
       title in your function's help text (i.e. a line which begins with
       a ` character and ends with a ' character." 
  (interactive "P")
  (if hide-all 
      (gre-hide-all)
    (let ((the-begin) (the-end) (modified (buffer-modified-p)))
      (save-excursion
        ;; FIXME: Find beginning, then use (scan-lists) to find end with error-trapping.
        (if (not (re-search-forward "^}" nil t))
            (error "Sorry, can't find the end of the function to hide.")
          (setq the-end (point)))
        (if (not (re-search-backward "^\\(cmd\\|sub\\).*$" nil t))
            (error "Sorry, can't find the beginning of the function to hide.")
          (setq the-begin (point))))
      (hide-region-body the-begin the-end)      
      (set-buffer-modified-p modified))))

(defun gre-hide-all (&optional quiet)
  "Hide all functions in current buffer, similarly to outline-mode.
Optional argument t (prefix) will make it quiet about error.  This should 
be used if you call this function in your gre-mode-hook such that it won't
complain when you load a gre file which contains no locally defined gre 
functions.

BUGS:  Will get confused if you have a string which looks like a function
       title in your function's help text (i.e. a line which begins with
       a ` character and ends with a ' character.)" 
  (interactive "P")
  (let ((the-end) (modified (buffer-modified-p)))
    (save-excursion
      (goto-char (point-min))
      (while (and (re-search-forward "^\\(cmd\\|sub\\).*$" nil t)
                  (re-search-forward "^}$" nil t)))
      (if (= (point) (point-min))
          (if (not quiet)
              (message "Sorry, can't find any functions to hide."))
        (forward-line 1)
        (setq the-end (point))
        (hide-region-body (point-min) the-end)
        (set-buffer-modified-p modified)))))

;;;----------
;;; font-lock stuff

(defvar gre-mode-system-face 'gre-mode-system-face
  "Face to use for gre-mode system calls.")

(defun gre-setup-font-lock ()
  (if (not (featurep 'font-lock))
      nil
    (cond 
     (gre-mode-is-XEmacs
      (make-face 'gre-mode-system-face)
      (set-face-foreground 'gre-mode-system-face "red" 'global nil 'append))
     (gre-mode-is-Emacs20
      (copy-face 'font-lock-warning-face 'gre-mode-system-face))
     (t
      ;; emacs-19:
      (if (not font-lock-face-attributes)
          (font-lock-make-faces))
      (if (not (assq 'gre-mode-system-face font-lock-face-attributes))
          (setq font-lock-face-attributes
                (append 
                 font-lock-face-attributes
                 '((gre-mode-system-face "red" nil t nil nil)))))))
    ;; FIXME: There is a better method for XEmacs... Does this one work?
    (make-local-variable 'font-lock-defaults)
    (setq font-lock-defaults
          '(gre-font-lock-keywords
            nil           ;;; Keywords only? No, let it do syntax via table.
            nil           ;;; case-fold?
            nil           ;;; Local syntax table.
            nil           ;;; Use `backward-paragraph' ? No
            ;;(font-lock-comment-start-regexp . "#")
            ;;(font-lock-mark-block-function . mark-paragraph)))
            ))))

(defconst gre-font-lock-keywords nil
  "Default expressions to fontify in gre-mode.")

;; (make-regexp 
;;  '("break" "else" "else if" "end if" "end while" "if" "quit" "return" "rpn" 
;;    "while"))

(setq
 gre-font-lock-keywords
 '((gre-font-lock-match-functions
    (0 font-lock-function-name-face)
    (1 font-lock-function-name-face nil t)
    (2 font-lock-function-name-face nil t))
   (gre-font-lock-system-commands
    (0 font-lock-function-name-face nil t)
    (1 font-lock-keyword-face nil t)
    (2 gre-mode-system-face prepend t))
   ;; quit return if else while sub do die last next 
   ("\\b\\(quit\\|while\\|until\\|unless\\|for\\|foreach\\|return\\|if\\|else\\|sub\\|cmd\\|do\\|die\\|last\\|next\\)\\b"
    . font-lock-keyword-face)
   ("$_[a-zA-Z0-9_]+" . font-lock-type-face)
   ("$[a-zA-Z0-9][_a-zA-Z0-9]* *=" . font-lock-reference-face)
   ("$[a-zA-Z0-9][_a-zA-Z0-9]*" . font-lock-variable-name-face)
   ))

(defconst gre-help-font-lock-keywords nil
  "Default expressions to fontify help in gre-mode.")

(setq gre-help-font-lock-keywords
      '(("$_[a-zA-Z0-9_]+" . font-lock-type-face)
        ("$[a-zA-Z0-9][_a-zA-Z0-9]* *=" . font-lock-reference-face)
        ("$[a-zA-Z0-9][_a-zA-Z0-9]*" . font-lock-variable-name-face)
        ("\\(@unit\\|@?default\\) +\\([a-zA-Z0-9.]+\\)" 
         (1 font-lock-keyword-face)
         (2 gre-mode-system-face))
        ("@param" . font-lock-keyword-face)
        ("^  .+" (0 font-lock-comment-face append t))
        ("^\\(Variable\\) $_[a-zA-Z0-9_]+ \\(.*\\)" 
         (1 font-lock-keyword-face append t)
         (2 font-lock-comment-face append t))))

(defun gre-font-lock-system-commands (limit)
  "Match gre system commands.
Return: match 0 as the declaration.
        match 1 as the system keyword.
        match 2 as the command text."
  ;; Prior imcomplete multi-line system command?
  (let ((start-point (point))
        (start-line (progn (beginning-of-line)(point)))
        (last-step-ok))
    (while (and (setq last-step-ok (= 0 (forward-line -1)))
                (progn (end-of-line)
                       (eq (preceding-char) ?\\))))
    (if last-step-ok (forward-line 1))
    (if (or (= (point) start-line)
            (not (looking-at
                 "\\(^\\|\C-m\\)[ \t]*\\(\\\\[^ ]+[ ]+=[ ]+\\)?\\(system\\)")))
        (goto-char start-point)))
  (if (re-search-forward "\\bsystem\\b" limit t)
      (let ((sys-b  (match-beginning 0))
            (sys-e  (match-end 0))
            decl-b decl-e com-b com-e)
        ;; goto bol to see if commented out
        (re-search-backward "^\\|\C-m" (point-min) t)
        (if (not (looking-at 
                  "\\(^\\|\C-m\\)[ \t]*\\(\\\\[^ ]+[ ]+=[ ]+\\)?\\(system\\)"))
            ;;                         ^^^^^^^^^^^^^^^^^^^^^^^^opt synonym
            (progn                      ; It was commented out or otherwise
              (goto-char sys-e)         ;  invalid
              (store-match-data (list nil nil))
              t)
          (setq decl-b (match-beginning 2))
          (setq decl-e (match-end 2))
          ;; <<EOF form?
          (if (re-search-forward 
               "\\(<<\"?EOF\"?\\)?\\(\n\\|\C-m\\)" limit t)
              (if (match-beginning 1)
                  (progn
                    (setq com-b (1+ sys-e))
                    (setq com-e (match-end 0))
                    (if (re-search-forward "\\(^\\|\C-m\\)EOF" limit t)
                        (setq com-e (match-end 0))))
                ;; Not an EOF form, skip all continuation lines
                (backward-char 1)
                (while (and (re-search-forward "$\\|\C-m" limit t)
                            (eq (preceding-char) ?\\)
                            (re-search-forward "^" limit t)))
                (setq com-b (1+ sys-e))
                (setq com-e (point))))
          (goto-char sys-e)
          (store-match-data
           (list decl-b decl-e sys-b sys-e com-b com-e))
          t))))

(defun gre-font-lock-match-functions (limit)
  "Match gre function declarations.
Return: match 0 as the declaration.
        match 1 as the opening bracket.
        match 2 as the closing bracket."
  (when (re-search-forward "^cmd " limit t)
    (re-search-forward "\\([^{#]+\\)\\({\\)" limit t)
    (let ((decl-b (match-beginning 1))
          (decl-e (match-end 1))
          (open-b (match-beginning 2))
          (open-e (match-end 2))
          close-b close-e)
      (goto-char open-b)
      (when (condition-case nil
                (goto-char (or (scan-sexps (point) 1) (point-max)))
              (error))
        (setq close-b (1- (point)))
        (setq close-e (1+ close-b)))
      (goto-char open-e)
      (store-match-data
       (list decl-b decl-e open-b open-e close-b close-e))
      t)))

;;;-------------
;;; hideshow stuff like hs-minor-mode

;;;FIXME: make gre function handle functions and related comments
;;;       simultaneously.

;;;FIXME: XEmacs doesn't have eval-after-load !

(eval-after-load 
    "hideshow"
  '(setq hs-special-modes-alist
         (append       
          hs-special-modes-alist
          '((gre-mode "cmd [^{]+ {" "}" nil 
                      gre-hs-forward-sexp-func
                      hs-c-like-adjust-block-beginning)))))

(defun gre-hs-forward-sexp-func (arg)
  "Moves point to end of the next function (with ARG 1)"
  (when (re-search-forward "^cmd [^{]+{" nil t)
    (forward-char -1)
    (forward-sexp arg)))

;; (defun gre-hs-hide-all ()
;;   (interactive)
;;   (let ((hs-minor-mode t))
;;     (hs-hide-all)))
;; 
;; (defun gre-hs-show-all ()
;;   (interactive)
;;   (let ((hs-minor-mode t))
;;     (hs-show-all)))
;; 
;; (defun gre-hs-show-function ()
;;   (interactive)
;;   (let ((hs-minor-mode t))
;;     (hs-show-block)))
;; 
;; (defun gre-hs-hide-function ()
;;   (interactive)
;;   (let ((hs-minor-mode t))
;;     (hs-hide-block)))

(defun gre-setup-hs ()
  (if (fboundp 'add-to-invisibility-spec)
      (add-to-invisibility-spec '(hs . t))) ;;hs invisible

  ;;; Use these if creating my own hideshow minor mode as part of gre-mode
  ;;(require 'hideshow)
  ;;(hs-grok-mode-type)

  ;; Rely on users to invoke hs-minor-mode by themselves, but modify it a
  ;; bit if minor-mode-overriding-map-alist is avialable (in Emacs-20.3 ?)
  (cond
   ((boundp 'minor-mode-overriding-map-alist)
    (make-local-variable 'gre-hs-minor-mode-map)
    (setq gre-hs-minor-mode-map (make-sparse-keymap))
;;  (define-key gre-hs-minor-mode-map "\C-ch" 'gre-hs-hide-function)
;;  (define-key gre-hs-minor-mode-map "\C-cs" 'gre-hs-show-function)
;;  (define-key gre-hs-minor-mode-map "\C-cH" 'gre-hs-hide-all)
;;  (define-key gre-hs-minor-mode-map "\C-cS" 'gre-hs-show-all)
;;  (define-key gre-hs-minor-mode-map [menu-bar Hide/Show]
;;    (cons "Hide/Show" (make-sparse-keymap "Hide/Show")))
;;  (define-key gre-hs-minor-mode-map [menu-bar Hide/Show hs-show-all]
;;    '("Show All" . gre-hs-show-all))
;;  (define-key gre-hs-minor-mode-map [menu-bar Hide/Show hs-hide-all]
;;    '("Hide All" . gre-hs-hide-all))
;;  (define-key gre-hs-minor-mode-map [menu-bar Hide/Show hs-show-block]
;;    '("Show Function" . gre-hs-show-function))
;;  (define-key gre-hs-minor-mode-map [menu-bar Hide/Show hs-hide-block]
;;    '("Hide Function" . gre-hs-hide-function))
    (define-key gre-hs-minor-mode-map [menu-bar Hide/Show]
      (cons "Hide/Show" (make-sparse-keymap "Hide/Show")))
    (define-key gre-hs-minor-mode-map [menu-bar Hide/Show hs-show-all]
      '("Show All Gre Functions" . hs-show-all))
    (define-key gre-hs-minor-mode-map [menu-bar Hide/Show hs-hide-all]
      '("Hide All Gre Functions" . hs-hide-all))
    (define-key gre-hs-minor-mode-map [menu-bar Hide/Show hs-show-block]
      '("Show This Gre Function" . hs-show-block))
    (define-key gre-hs-minor-mode-map [menu-bar Hide/Show hs-hide-block]
      '("Hide This Gre Function" . hs-hide-block))
    
    (make-variable-buffer-local 'minor-mode-overriding-map-alist)
    (or (assq 'hs-minor-mode minor-mode-overriding-map-alist)
        (setq minor-mode-overriding-map-alist
              (cons (cons 'hs-minor-mode gre-hs-minor-mode-map)
                    minor-mode-overriding-map-alist))))))


;;;-------------
;;; imenu stuff

;; Instead of setting the variable imenu-create-index-function to
;; imenu--create-gre-index, I could set the following REGEXP variable:
;; 
;;  (setq imenu-generic-expression
;;        '((nil "^cmd \\([^(]+\\)" 1)
;;          ("Variables" "^\\($[a-zA-Z_]+\\) [+-/*]?=" 1)))
;; 
;; and make a simple `imenu-prev-index-position-function' to move backwards
;; to these regexps.  (See gre-imenu-prev-index-position-function below).

;;FIXME:  XEmacs consideration here!
(if gre*use-imenu
    (require 'imenu))
(if (and (= emacs-major-version 20)
         (<= emacs-minor-version 2))
    (defun imenu-add-to-menubar (name)
      "Adds an `imenu' entry to the menu bar for the current buffer.
NAME is a string used to name the menu bar item.
See the command `imenu' for more information."
      (interactive "sImenu menu item name: ")
      (if (or (and (equal imenu-create-index-function 
                          'imenu-default-create-index-function)
                   imenu-generic-expression)
              (fboundp imenu-create-index-function))
          (let ((newmap (make-sparse-keymap))
                (menu-bar (lookup-key (current-local-map) [menu-bar])))
            (define-key newmap [menu-bar]
              (append (make-sparse-keymap) menu-bar))
            (define-key newmap [menu-bar index]
              (cons name (nconc (make-sparse-keymap "Imenu")
                                (make-sparse-keymap))))
            (use-local-map (append newmap (current-local-map)))
            (add-hook 'menu-bar-update-hook 'imenu-update-menubar))
        (error "The mode `%s' does not support Imenu" mode-name))))

(defun gre-imenu-prev-index-position-function ()
  (re-search-backward 
  "\\(^\\(cmd\\|sub\\) +\\([^(]+\\)\\)\\|\\(^[ \t]*\\($[a-zA-Z0-9_]+\\) *=\\)"
   nil t))

(defvar gre-imenu-counter nil "gre-mode internal variable for imenu support")

(defun imenu--create-gre-index ()
    (save-match-data
      (save-excursion
        (let ((index-alist '())
              (index-var-alist '())
              prev-pos)
          (setq gre-imenu-counter -99) ;IDs menu entries starting at -100
          (goto-char (point-max))
          (imenu-progress-message prev-pos 0 t)
          (while 
              (re-search-backward 
               "\\(^\\(cmd\\|sub\\) +\\([^(]+\\)\\)\\|\\(^[ \t]*\\($[a-zA-Z0-9_]+\\) *=\\)" 
               nil t)
            (imenu-progress-message prev-pos nil t)
            (let ((marker (make-marker)))
              (set-marker marker (point))
              (cond
               ((match-beginning 1)     ;function
                (push (cons (match-string 3) marker) index-alist))
               (t                       ;variable
                (push (cons (match-string 5) marker) index-var-alist)))))
          (imenu-progress-message prev-pos 100 t)
          (cond
           ((and index-var-alist
                 (elt index-var-alist 5))
            (push (cons "Variables"
           ;;Or this:   (sort index-var-alist 'gre-imenu-label-cmp))
                        index-var-alist) 
                  index-alist))
           (index-var-alist
            (append index-var-alist index-alist))
           (t
            index-alist))))))

(defun gre-imenu-label-cmp (el1 el2)
  "Predicate to compare labels in lists."
  (string< (car el1) (car el2)))

;;;----------
;;; Gre internals

;; FIXME: Do I still use this?
(defun gre-replace-within-string (in-string from to) 
  (save-match-data 
    (while (string-match from in-string)
      (setq in-string (replace-match to nil t in-string)))
    in-string))

(defun gre*date-function ()
  "Adapted from objective-C-mode.  Add name, author and date on current line."
  (let* ((u (- 63 (length (user-full-name)) (length (gre*date)))))
    (insert "# ")
    (when (not (string-equal (user-full-name) ""))
      (setq u (- u 1))
      (insert (user-full-name) " "))
    (insert-char 32 u)
    (insert (gre*date))))
    
(defun gre*date ()
  "Return the current date in an EB Signal standard form.
Code from objective-C-mode."
  (concat (substring (current-time-string) -4 nil) "-"
	  (substring (current-time-string) 4 7) "-"
	  (substring (current-time-string) 8 10)))

;;;-------------
;;; Setup auto-mode-alist
(if (not (assoc '"\\.gre$" auto-mode-alist))
    (setq auto-mode-alist (cons '("\\.gre$" . gre-mode) auto-mode-alist)))
(provide 'gre-mode)
;;; gre-mode.el ends here


