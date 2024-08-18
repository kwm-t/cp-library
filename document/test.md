```math
        2^{({k+1})/2}  H_{k+1}(H_{k+1}u \odot H_{k+1}v)
        =
        2^{({k+1})/2}
        H_{k+1}
        \left(
            \frac{1}{\sqrt{2}}
            \begin{bmatrix}
                 H_{k}u_{0} + H_{k}u_{1} \cr
                H_{k}u_{0} - H_{k}u_{1}
            \end{bmatrix}
            \odot
            \frac{1}{\sqrt{2}}
            \begin{bmatrix}
                    H_{k}v_{0} + H_{k}v_{1} \cr
                    H_{k}v_{0} - H_{k}v_{1}
            \end{bmatrix}
        \right)
        =
        2^{({k+1})/2} 
        \frac{1}{\sqrt{2}}
        \begin{bmatrix}
            H_{k} & H_{k} \cr
            H_{k} & -H_{k}
        \end{bmatrix}
        \left(
            \frac{1}{\sqrt{2}}
            \begin{bmatrix}
                H_{k}u_{0} + H_{k}u_{1} \cr
                H_{k}u_{0} - H_{k}u_{1}
            \end{bmatrix}
            \odot
            \frac{1}{\sqrt{2}}
            \begin{bmatrix}
                H_{k}v_{0} + H_{k}v_{1} \cr
                H_{k}v_{0} - H_{k}v_{1}
            \end{bmatrix}
        \right)
        =
        \frac{2^{{k}/2}}{2} 
        \begin{bmatrix}
            H_{k} & H_{k} \cr
            H_{k} & -H_{k}
        \end{bmatrix}
        \begin{bmatrix}
            (H_{k}u_{0} + H_{k}u_{1})\odot(H_{k}v_{0} + H_{k}v_{1}) \cr
            (H_{k}u_{0} - H_{k}u_{1})\odot(H_{k}v_{0} - H_{k}v_{1})
            \end{bmatrix}
        =
        2^{{k}/2} 
            \begin{bmatrix}
                H_{k}(H_{k}u_{0}\odot H_{k}v_{0}) + H_{k}(H_{k}u_{1}\odot H_{k}v_{1}) \cr
                H_{k}(H_{k}u_{0}\odot H_{k}v_{1}) + H_{k}(H_{k}u_{1}\odot H_{k}v_{0})
            \end{bmatrix}
        =
            \begin{bmatrix}
            u_{0}*v_{0}+u_{1}*v_{1} \cr
            u_{0}*v_{1}+u_{1}*v_{0}
            \end{bmatrix}
        =
        u*v
```