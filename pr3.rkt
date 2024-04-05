#lang racket

;; Example environment with a user-defined function 'foo'
(define env '((foo . (lambda (x y) (/ (+ x y) 2))) (a . 12) (b . 5.2)))

(define (evalu8 expr env)
  (cond
    ;; Case 1: Expr is a number
    ((number? expr) expr)
    
    ;; Case 2: Expr is a symbol, look it up in the environment
    ((symbol? expr)
     (let ((binding (lookup expr env)))
       (if (pair? binding)
           (evalu8 binding env)
           binding)))
    
    ;; Case 3: Expr is a list
    ((pair? expr)
     (let ((operation (car expr))
           (operands (cdr expr)))
       (cond
         ;; Sub-case: operation is a lambda expression, evaluate it directly
         ((eq? operation 'lambda) (apply (eval expr) '()))
         
         ;; Sub-case: operation is a symbol, indicating a function call
         (else
          (let ((func (evalu8 operation env))
                (args (map (lambda (arg) (evalu8 arg env)) operands)))
            (if (procedure? func)
                (apply func args)
                ;; This case handles user-defined functions or lambdas in the environment
                (evalu8 (list func args) env)))))))
    
    ;; Default case: invalid expression
    (else (error "Invalid expression" expr))))

(define (lookup var env)
  (cond ((null? env) (error "Unbound variable" var))
        ((equal? (caar env) var) (cdar env))
        (else (lookup var (cdr env)))))



;; Testing the evalu8 function with a user-defined function
;;(evalu8 '(foo 5 9) env) ;; Should return 7

;; Testing the evalu8 function with a lambda expression in function position
;;(evalu8 '((lambda (x y) (* x (+ x y))) 2 3) env) ;; Should return 10
