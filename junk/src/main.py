# scrap the data from sky-map
# categorize by constellations (caching stars from current constellation)
# find pattern from which star's pos can be found (sphere coordinate system)
# if i have one pos, then i can search for other stars (brighter spots on frame with certain characterictics)

# maybe i can calc pos of star with my iphone pos and iphone's compass direction
# if i have a star pos, then i can search for this pos in db

# pos of device and current time/date


# get device pos and orientation
# get all stars that should be visible
# calc their pos on screen
# display them

import os
import subprocess

import cv2
import numpy
import pygame


# WIDTH, HEIGHT = 1920 // 2, 1080 // 2
# WIDTH, HEIGHT = 1080, 1920
WIDTH, HEIGHT = 1920, 1080
SURFACE = pygame.HWSURFACE | pygame.DOUBLEBUF | pygame.RESIZABLE

pygame.init()
pygame.mixer.init()
window = pygame.display.set_mode((HEIGHT // 2, WIDTH // 2), SURFACE)
pygame.display.set_caption("Powered by XYCORP Labs")


class Video(pygame.sprite.Sprite):
    FFMPEG_BIN = "/opt/homebrew/bin/ffmpeg"

    def __init__(self, filename, rect, fps=25):
        pygame.sprite.Sprite.__init__(self)
        cmd = [
            self.FFMPEG_BIN,
            "-loglevel", "quiet",
            "-i", filename,
            "-f", "image2pipe",
            "-s", "%dx%d" % (rect.width, rect.height),
            "-pix_fmt", "rgb24",
            "-vcodec", "rawvideo", "-"
        ]
        self.bytes_per_frame = rect.width * rect.height * 3
        self.proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, bufsize=self.bytes_per_frame * 3)
        self.image = pygame.Surface((rect.width, rect.height), pygame.HWSURFACE)
        self.k = 0
        self.rect = self.image.get_rect()
        self.rect.x = rect.x
        self.rect.y = rect.y
        self.last_at = 0
        self.frame_delay = 1000 / fps
        self.is_video_stopped = False

    def update(self):
        if not self.is_video_stopped:
            tnow = pygame.time.get_ticks()
            if tnow > self.last_at + self.frame_delay:
                self.last_at = tnow
                try:
                    raw_img = self.proc.stdout.read(self.bytes_per_frame)
                    self.image = pygame.image.frombuffer(raw_img, (self.rect.width, self.rect.height), "RGB")
                    # view = pygame.surfarray.array3d(self.image)
                    # view = view.transpose([1, 0, 2])
                    # proc_image = cv2.cvtColor(view, cv2.COLOR_RGB2GRAY)
                    # cv2.imwrite(f"frames/frame_{self.k}.jpg", proc_image)
                    self.k += 1
                except:
                    self.image = pygame.Surface((self.rect.width, self.rect.height), pygame.HWSURFACE)
                    self.image.fill((0, 0, 0))
                    self.is_video_stopped = True


def draw_box(frame, text, pos): # (60, 0, 197)
    cv2.rectangle(frame, (pos[0] - 105, pos[1] - 20), (pos[0] - 20, pos[1]), (0, 255, 0), 1)
    cv2.line(frame, (pos[0] - 20, pos[1] - 6), (pos[0] - 3, pos[1]), (0, 255, 0), 1)
    cv2.putText(frame, text, (pos[0] - 100, pos[1] - 6), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (0, 255, 0), 1)


def run():
    video = Video("5939440-hd_1920_1080_30fps.mp4", pygame.Rect(0, 0, WIDTH, HEIGHT))
    group = pygame.sprite.Group()
    group.add(video)

    font = pygame.font.Font(pygame.font.get_default_font(), 11)
    text = font.render("SIRIUS", True, (197, 0, 60))

    text_rect = text.get_rect()
    text_rect.topleft = (100, 100)

    rect = pygame.Rect(text_rect.left - 5, text_rect.top - 5, text_rect.width + 2 * 5, text_rect.height + 2 * 5)

    is_running = True
    clock = pygame.time.Clock()
    try:
        while is_running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT: is_running = False

            group.update()
            window.fill((0, 0, 0))
            group.draw(window)

            image = pygame.surfarray.array3d(video.image)
            image = image.transpose([1, 0, 2])
            rimage = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
            gimage = cv2.cvtColor(rimage, cv2.COLOR_BGR2GRAY)

            # th, bimage = cv2.threshold(gimage, 140, 255, cv2.THRESH_TOZERO_INV)
            bimage = cv2.adaptiveThreshold(gimage, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY_INV, 15, 4)
            cunts = cv2.findContours(bimage, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)[-2]

            cunts = [c for c in cunts if 20 < cv2.contourArea(c) < 60] # there are probably better values 
            # print(f"lights={len(lights)}")

            moments = [cv2.moments(c) for c in cunts]
            cents = [(int(m["m10"] / (m["m00"] + 1e-5)), int(m["m01"] / (m["m00"] + 1e-5))) for m in moments] # (x, y) pos

            for c in cents:
                # localize cent
                draw_box(rimage, "CMa (Sirius)", c)
                pygame.draw.circle(video.image, (0, 255, 0), c, 5, 1)

                # cv2.rectangle(rimage, (c[0] - 2, c[1] - 2), (c[0] + 5, c[1] + 5), (60, 0, 197), 1)
                # cv2.circle(rimage, c, 5, (60, 0, 197))
                # cv2.rectangle(rimage, (c[0] - 105, c[1] - 20), (c[0] - 20, c[1]), (0, 255, 0), 1)
                # cv2.line(rimage, (c[0] - 20, c[1] - 6), (c[0], c[1]), (0, 255, 0), 1)
                # cv2.putText(rimage, "SIRIUS", (c[0] - 100, c[1] - 6), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 1)

                # cv2.circle(gimage, c, 5, (60, 0, 197))
                # cv2.circle(bimage, c, 5, (60, 0, 197))

            if video.k == 2:
                cv2.imwrite(f"frames/frame_{video.k}.jpg", rimage)
                cv2.imwrite(f"frames/frame_g{video.k}.jpg", gimage)
                cv2.imwrite(f"frames/frame_b{video.k}.jpg", bimage)
                # break

            pygame.display.flip()
            clock.tick_busy_loop(30)
    except KeyboardInterrupt:
        pass

    pygame.quit()


def del_frames():
    for file in os.listdir("frames"):
        path = os.path.join("frames/", file)
        os.remove(path)


if __name__ == "__main__":
    run()