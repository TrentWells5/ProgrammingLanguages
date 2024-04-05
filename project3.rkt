#lang racket
(define env '((a . 12) (b . 5.2) (foo . (lambda (x y) (/ (+ x y) 2)))))

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

; Testing
(println(evalu8 5 '((a . 5)))) ; Expect 5
(println(evalu8 'a '((a . 5)))) ; Expect 5
; next line purposely throws and error, so it's commented out
; (println(evalu8 'b '((a . 5)))) ; Error: b is not bound (or similar)
(println(evalu8 '(+ 5 3) '((a . 5)))) ; Expect 8
(println(evalu8 '(equal? 5 3) '((a . 5)))) ; Expect #f
(println(evalu8 '(equal? (+ a b) 3) '((a . 1) (b . 2)))) ; Expect #t
(println(evalu8 '(foo 5 9) env))  ; Expect 7
(println(evalu8 '((lambda (x y) (* x (+ x y))) 2 3) env))  ; Expect 10
