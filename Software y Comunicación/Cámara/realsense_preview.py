import pyrealsense2 as rs
import numpy as np
import cv2, time

W, H, FPS = 1280, 720, 30

pipe = rs.pipeline()
cfg  = rs.config()
cfg.enable_stream(rs.stream.depth, W, H, rs.format.z16, FPS)
cfg.enable_stream(rs.stream.color, W, H, rs.format.bgr8, FPS)
profile = pipe.start(cfg)

align = rs.align(rs.stream.color)
depth_scale = profile.get_device().first_depth_sensor().get_depth_scale()
print("Depth scale (m per unit):", depth_scale)

t0, frames_count = time.time(), 0
try:
    while True:
        frames = pipe.wait_for_frames()
        frames = align.process(frames)

        depth = frames.get_depth_frame()
        color = frames.get_color_frame()
        if not depth or not color:
            continue

        depth_np = np.asanyarray(depth.get_data())
        color_np = np.asanyarray(color.get_data())
        
        # Voltear verticalmente la imagen de color y la de profundidad
        # Esto solo cuando la camara esté montada en robot
        color_np = cv2.flip(color_np, 0)
        depth_np = cv2.flip(depth_np, 0)
        #----------------------------------------
        
        # Distancia en el centro
        cx, cy = W//2, H//2
        dist_m = depth_np[cy, cx] * depth_scale
        cv2.circle(color_np, (cx, cy), 4, (0,255,0), -1)
        cv2.putText(color_np, f"{dist_m:.2f} m", (10,30),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (0,255,0), 2)

        # Visualización del depth (colorizado)
        depth_vis = cv2.convertScaleAbs(depth_np, alpha=0.03)
        depth_vis = cv2.applyColorMap(depth_vis, cv2.COLORMAP_JET)

        cv2.imshow("Color", color_np)
        cv2.imshow("Depth", depth_vis)

        if cv2.waitKey(1) == 27:  # ESC para salir
            break
finally:
    pipe.stop()
    cv2.destroyAllWindows()
