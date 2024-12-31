// 倒计时功能
function countdown() {
    const year = new Date().getFullYear() + 1;
    const targetDate = new Date(`January 1, ${year} 00:00:00`);
    const now = new Date();
    const difference = targetDate - now;

    const days = Math.floor(difference / (1000 * 60 * 60 * 24));
    const hours = Math.floor((difference % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60));
    const minutes = Math.floor((difference % (1000 * 60 * 60)) / (1000 * 60));
    const seconds = Math.floor((difference % (1000 * 60)) / 1000);

    document.getElementById('days').innerText = days < 10 ? '0' + days : days;
    document.getElementById('hours').innerText = hours < 10 ? '0' + hours : hours;
    document.getElementById('minutes').innerText = minutes < 10 ? '0' + minutes : minutes;
    document.getElementById('seconds').innerText = seconds < 10 ? '0' + seconds : seconds;

    setTimeout(countdown, 1000);
}

countdown();

// 几何粒子效果
class Particle {
    constructor(x, y) {
        this.x = x;
        this.y = y;
        this.vx = (Math.random() - 0.5) * 5;
        this.vy = (Math.random() - 0.5) * 5;
        this.radius = Math.random() * 5 + 2;
        this.color = `hsl(${Math.random() * 360}, 50%, 50%)`;
    }
    update() {
        this.x += this.vx;
        this.y += this.vy;
        // 检查边界，如果超出则反向反弹
        if (this.x + this.vx < 0 || this.x + this.vx > canvas.width) {
            this.vx = -this.vx;
        }
        if (this.y + this.vy < 0 || this.y + this.vy > canvas.height) {
            this.vy = -this.vy;
        }
    }
    draw(ctx) {
        ctx.beginPath();
        ctx.arc(this.x, this.y, this.radius, 0, Math.PI * 2);
        ctx.fillStyle = this.color;
        ctx.fill();
    }
}

const canvas = document.getElementById('particleCanvas');
const ctx = canvas.getContext('2d');

const particles = [];

function createParticles() {
    for (let i = 0; i < 100; i++) {
        const x = Math.random() * canvas.width;
        const y = Math.random() * canvas.height;
        particles.push(new Particle(x, y));
    }
}

function updateParticles() {
    particles.forEach(particle => {
        particle.update();
    });
}

function drawParticles() {
    particles.forEach(particle => {
        particle.draw(ctx);
    });
}

function drawConnections() {
    for (let i = 0; i < particles.length; i++) {
        for (let j = i + 1; j < particles.length; j++) {
            const p1 = particles[i];
            const p2 = particles[j];
            const dx = p1.x - p2.x;
            const dy = p1.y - p2.y;
            const distance_sq = dx * dx + dy * dy;
            const threshold_sq = 10000; // 100 pixels
            if (distance_sq < threshold_sq) {
                ctx.beginPath();
                ctx.moveTo(p1.x, p1.y);
                ctx.lineTo(p2.x, p2.y);
                ctx.strokeStyle = 'rgba(255,255,255,0.5)';
                ctx.stroke();
            }
        }
    }
}

function animate() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    updateParticles();
    drawParticles();
    drawConnections();
    requestAnimationFrame(animate);
}

// 初始化
canvas.width = window.innerWidth;
canvas.height = window.innerHeight;
createParticles();
animate();