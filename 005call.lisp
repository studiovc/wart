;; Override call and apply using extensible coerce

; Because (coerce x 'function) evals x
(defmacro fslot(f)
  (if (and (atom f) (fboundp f))
    `(function ,f)
    f))

(ignore-redef
  (defun call(f &rest args)
    (apply (wart-coerce (fslot f) 'function)
           args)))

(defmacro wart-apply(f &rest args)
  `(call (apply-fn (wart-coerce (fslot ,f) 'function))
         ,@args))
(defover apply wart-apply)

(defmacro defcall(type arg &rest body)
  `(defcoerce ',type 'function
     (lambda(,arg)
       ,@body)))



(defmacro call*(f &rest args)
  (if (and (atom f) (macp f))
    `(,f ,@args)
    `(call (fslot ,f) ,@args)))

(defun call-macro(macro &rest args)
  (eval (macex `(,macro ,@args))))

(defun macro-wrapper(macro)
  (lambda(&rest args)
    (eval (macex `(,macro ,@args)))))

(defcoerce 'macro 'function
  [macro-wrapper _])



;; Internals

(defun apply-fn(f)
  (lambda(&rest args)
    (apply f (inline-last args))))

(defun inline-last(xs)
  (if (not (consp xs))
    xs
    (if (cdr xs)
      (cons (car xs) (inline-last (cdr xs)))
      (if (consp (car xs))
        (car xs)
        xs))))
