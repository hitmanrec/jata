<template>
	<div class="colored-slider">
		<div class="colored-slider__track" role="slider">
			<input
				class="colored-slider__range"
				type="range"
				:min="minValue"
				:max="maxValue"
				step="1"
				v-model.number="sliderValue"
				:disabled="!editable"
				aria-hidden="true"
			/>
			<div class="colored-slider__ticks">
			<span
				v-for="mark in tickMarks"
				:key="mark"
				class="colored-slider__tick"
				:style="{ left: `${((mark - minValue) / (maxValue - minValue)) * 100}%` }"
			>
			<span :style="{ opacity:  mark > sliderValue && mark > 0 && mark < tickMarks.length - 1 ? 1 : 0 }" class="colored-slider__tick-symbol">♦</span>
			<span class="colored-slider__tick-number">{{ mark }}</span>

			</span>
      	</div>
		</div>
		
	</div>
</template>

<script>
export default {
	name: 'colored-slider',
	props: {
		minValue: {
			type: Number,
			required: true
		},
		maxValue: {
			type: Number,
			required: true
		},
		currentValue: {
			type: Number,
			default: 0
		},
		editable: {
			type: Boolean,
			default: false
		}
	},
	data() {
		return {
			sliderValue: this.currentValue === undefined ? (this.minValue + this.maxValue) / 2 : this.currentValue
		}
	},
	watch: {
		currentValue(newVal) {
			this.sliderValue = newVal
		},
		sliderValue(newVal) {
			this.$emit('update:currentValue', newVal)
		}
	},
	computed: {
		tickMarks() {
			const marks = [];
			const interval = 1;
			for (let value = this.minValue; value <= this.maxValue; value += interval) {
				marks.push(value);
			}
			return marks;	
		}
	}
}
</script>

<style scoped>

.colored-slider{
	margin: 5px;
}
.colored-slider__range{
	width: 100%;
	-webkit-appearance: none; 
	appearance: none;
	background: transparent;
	height: 11px;
	border-radius: 5px;
	border: 1px solid var(--accent-col);
}

.colored-slider__track {
  position: relative;
}

.colored-slider__ticks {
  pointer-events: none;
  font-size: 0.8em;

  opacity: 0.7;
}

.colored-slider__tick {
  position: absolute;
  background: none;
  display: flex;
  flex-direction: column;
  align-items: center;
  transform: translate(-50%, -59%);

}

/* The slider handle (use -webkit- (Chrome, Opera, Safari, Edge) and -moz- (Firefox) to override default look) */
.colored-slider__range::-webkit-slider-thumb {
  -webkit-appearance: none; /* Override default look */
  appearance: none;
  border: none;
  background: none; /* Green background */
}

.colored-slider__range::-moz-range-thumb {
  border: none;
  background: none; /* Green background */
}

.colored-slider__range::-webkit-slider-progress{
	background: var(--accent-col);
}

.colored-slider__range::-moz-range-progress{
	background: var(--accent-col);
	height: 100%;
	border-radius: var(--small-border-rad);
	transition: background 0.2s ease, width 0.2s ease;
}
</style>