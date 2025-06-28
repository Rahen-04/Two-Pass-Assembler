
    ldc 0                  ; Initialize first Fibonacci number (fib_0 = 0)
    stl 0                  ; Store fib_0 in local variable (offset 0)
    ldc 1                  ; Initialize second Fibonacci number (fib_1 = 1)
    stl 1                  ; Store fib_1 in local variable (offset 1)

    ldc 2                  ; Start loop counter at 2
    stl 2                  ; Store loop counter in local variable (offset 2)

    ldl 2                  ; Load counter
    ldc n                  ; Load n
    sub                    ; Compare counter and n
    brz fib_done           ; If counter == n, we're done

fib_loop:
    ldl 0                  ; Load fib_0
    ldl 1                  ; Load fib_1
    add                    ; fib_next = fib_0 + fib_1
    stl 3                  ; Store fib_next in a temporary variable (offset 3)

    ldl 1                  ; Load fib_1
    stl 0                  ; Update fib_0 = fib_1

    ldl 3                  ; Load fib_next
    stl 1                  ; Update fib_1 = fib_next

    ldl 2                  ; Load counter
    ldc 1
    add                    ; counter++
    stl 2                  ; Update counter

    ldl 2                  ; Load updated counter
    ldc n                  ; Load n
    sub                    ; Compare counter and n
    brz fib_done           ; If counter == n, we're done

    br fib_loop            ; Otherwise, repeat the loop

fib_done:
    ldl 1                  ; Load the final Fibonacci number
    ldc fib_result ; Load the address of fib_result 
    stnl 0         ; Store result in fib_result

    HALT                   ; End program

n:        SET 10            ; Define the Fibonacci position (e.g., n = 6)
fib_result: data 0         ; Placeholder for the result