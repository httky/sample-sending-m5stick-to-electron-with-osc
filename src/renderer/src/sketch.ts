import p5 from 'p5'
import { Osc } from './osc'

export const sketch = (p: p5): void => {
	let oscText: string = ''
	const osc: Osc = new Osc()
	let baseRadius: number = 220
	let gyroX: number = 0
	let gyroY: number = 0
	let gyroZ: number = 0
	let isPressed: boolean = false

	/**
	 * Setup
	 */
	p.setup = (): void => {
		p.createCanvas(p.windowWidth, p.windowHeight)
		// OSC受信イベント
		osc.on(osc.MESSAGE, (address: string, args: any[]) => {
			oscText = `address: ${address}\nargs:\n${JSON.stringify(args)}\n`
			console.log(address, args)

			if (address === '/m5stick/gyro') {
				console.log('gyro: ', args)
				gyroX = args[0] as number
				gyroY = args[1] as number
				gyroZ = args[2] as number
			}

			if (address === '/m5stick/pressed') {
				isPressed = args[0] === 1
			}
		})
	}

	/**
	 * Draw
	 */
	p.draw = (): void => {
		p.background(250)
		p.noStroke()
		p.fill(0, 0, 200)
		p.textAlign(p.LEFT, p.CENTER)
		p.text(`x:${gyroX}`, 30, 0, p.width - 20, p.height - 20)
		p.text(`y:${gyroY}`, 30, 20, p.width - 20, p.height - 20)
		p.text(`z:${gyroZ}`, 30, 40, p.width - 20, p.height - 20)
		let radius = isPressed ? baseRadius * 1.4 : baseRadius
		p.ellipse(p.width / 2, p.height / 2, radius, radius)
	}

	/**
	 * Window Resized
	 */
	p.windowResized = (): void => {
		p.resizeCanvas(p.windowWidth, p.windowHeight)
	}
}
