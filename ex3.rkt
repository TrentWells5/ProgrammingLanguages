#lang racket
; last function
(define (last lst)
  (if (null? (cdr lst))
      (car lst)
      (last (cdr lst))))

; make-line function
(define (make-line a b)
  (lambda (x) (+ (* a x) b)))

; split function
(define (split lst)
  (let split-helper ((lst lst) (symbols '()) (numbers '()))
    (cond ((null? lst) (list (reverse symbols) (reverse numbers)))
          ((symbol? (car lst)) (split-helper (cdr lst) (cons (car lst) symbols) numbers))
          ((number? (car lst)) (split-helper (cdr lst) symbols (cons (car lst) numbers)))
          (else (split-helper (cdr lst) symbols numbers)))))


; count function
(define (count pred lst)
  (define (count-helper lst acc)
    (if (null? lst)
        acc
        (count-helper (cdr lst) (if (pred (car lst)) (+ acc 1) acc))))
  (count-helper lst 0))


; compose function
(define (compose f g)
  (lambda (x) (f (g x))))

; defined cadr and caddr
(define cadr (compose car cdr))
(define caddr (compose cadr cdr))
