<template>
	<div class="radio-row" role="radiogroup">
		<label
		v-for="(option, index) in possibleValues"
		:key="option"
		class="radio-pill"
		:style="colorPalette.length > 1 ? { '--pill-color': colorPalette[index % colorPalette.length] } :  { '--pill-color': colorPalette[0] }"
		:class="{ 'radio-pill--active': option === selectedValue,
			'radio-pill--first': possibleValues.length > 1 && index === 0,
			'radio-pill--middle': possibleValues.length > 1 && index !== 0 && index !== possibleValues.length - 1,
			'radio-pill--last': possibleValues.length > 1 && index === possibleValues.length - 1
		}"
		>
			<input
				class="radio-pill__input"
				type="radio"
				:value="option"
				v-model="selectedValue"
			/>
			<span class="radio-pill__label">{{ option }}</span>
		</label>
	</div>
</template>

<script>
export default {
	name: 'radio-row',
	props: {
		possibleValues: {
			type: Array,
			required: true
		},
		currentValue: {
			type: String,
			default: ''
		},
		colorPalette: {
			type: Array,
			default: () => []
		}
	},
	data() {
		return {
			selectedValue: (this.currentValue !== '' && this.possibleValues.includes(this.currentValue)) 
			? this.currentValue 
			: this.possibleValues[0]
		}
	},
	watch: {
		currentValue(newVal) {
			if (newVal !== this.selectedValue) {
				this.selectedValue = newVal;
			}
		},
		selectedValue(newVal) {
			this.$emit('update:currentValue', newVal)
		}
	}
};
</script>

<style scoped>
.radio-row {
  display: flex;
  flex-wrap: wrap;
  align-items: stretch;
  border: 1px solid var(--accent-col);
  border-radius: var(--big-border-rad);
  padding: 2px;
}

.radio-pill {
  position: relative;
  display: inline-flex;
  flex: 1 1 0;
  align-items: center;
  justify-content: center;
  background: none;
  color: var(--main-text-col);
  font-weight: 500;
  cursor: pointer;
  padding: 5px 10px;
  transition: background 0.2s ease, border-color 0.2s ease, color 0.2s ease,
    box-shadow 0.2s ease;
}

.radio-pill--active {
  background: var(--pill-color);
  color: var(--dark-bg-col);
}

.radio-pill--first {
  border-top-left-radius: var(--big-border-rad);
  border-bottom-left-radius: var(--big-border-rad);
}

.radio-pill--middle {
  border-left: 1px solid var(--accent-col);
}

.radio-pill--last {
  border-top-right-radius: var(--big-border-rad);
  border-bottom-right-radius: var(--big-border-rad);
  border-left: 1px solid var(--accent-col);
}


.radio-pill__input {
  position: absolute;
  inset: 0;
  opacity: 0;
  cursor: pointer;
}

.radio-pill__label {
  pointer-events: none;
}

.radio-pill:focus-within {
  border-color: var(--accent-col);
}
</style>