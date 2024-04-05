#lang racket
(define env '((a . 12) (b . 5.2)))

(define (evalu8 expression env)
  (if (pair? expression)  ; Check if the expression is a list (for operations).
      (let ((op (car expression))
            (arg1 (evalu8 (cadr expression) env))
            (arg2 (evalu8 (caddr expression) env)))
        (cond ((eq? op '+) (+ arg1 arg2))
              ((eq? op '*) (* arg1 arg2))
              ((eq? op 'equal?) (= arg1 arg2))
              ((eq? op 'less?) (< arg1 arg2))
              (else (error "Unknown operation"))))
      (if (number? expression)  ; Check if it's a numerical literal.
          expression
          (let ((binding (assoc expression env)))
            (if binding
                (cdr binding)
                (error "Error: Identifier not bound" expression))))))  ; Error for unbound identifiers.
