```math
    \begin{aligned} 
        2^{({k+1})/2}  H_{k+1}(H_{k+1}u \odot H_{k+1}v)
        &=
        \frac{2^{{k}/2}}{2} 
        \left[
            \begin{array}{cc}
            H_{k} & H_{k} \cr
            H_{k} & -H_{k}
            \end{array}
        \right]
        \left(
            \begin{array}{cc}
            (H_{k}u_{0} + H_{k}u_{1})\odot(H_{k}v_{0} + H_{k}v_{1}) \cr
            (H_{k}u_{0} - H_{k}u_{1})\odot(H_{k}v_{0} - H_{k}v_{1})
            \end{array}
        \right)
        \\\\
        &=
        2^{{k}/2} 
        \left(
            \begin{array}{cc}
                H_{k}(H_{k}u_{0}\odot H_{k}v_{0}) + H_{k}(H_{k}u_{1}\odot H_{k}v_{1}) \cr
                H_{k}(H_{k}u_{0}\odot H_{k}v_{1}) + H_{k}(H_{k}u_{1}\odot H_{k}v_{0})
            \end{array}
        \right)
        \\\\
        &=
        \left(
                \begin{array}{cc}
                u_{0}*v_{0}+u_{1}*v_{1} \cr
                u_{0}*v_{1}+u_{1}*v_{0}
            \end{array}
        \right)
        \\\\
        &=
        u*v
    \end{aligned}
```