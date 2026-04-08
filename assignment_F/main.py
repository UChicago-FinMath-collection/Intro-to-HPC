import time

# Julia set parameters
c_real = -0.7
c_imag = 0.27015
x_min, x_max = -1.5, 1.5
y_min, y_max = -1.5, 1.5
width = 1000
height = 1000
max_iter = 300

print(f"Generating Julia set ({width}x{height}, max_iter={max_iter})...")

start = time.time()

result = [[0] * width for _ in range(height)]

for row in range(height):
    for col in range(width):
        # map pixel to complex plane
        zx = x_min + (x_max - x_min) * col / (width - 1)
        zy = y_min + (y_max - y_min) * row / (height - 1)

        iteration = 0
        while zx * zx + zy * zy < 4.0 and iteration < max_iter:
            # z = z^2 + c
            tmp = zx * zx - zy * zy + c_real
            zy = 2.0 * zx * zy + c_imag
            zx = tmp
            iteration += 1

        result[row][col] = iteration

end = time.time()

print(f"Time taken: {end - start:.4f} seconds")
