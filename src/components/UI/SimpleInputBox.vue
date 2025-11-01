<template>
	<input type="text" class="base-input" :value="modelValue" @input="updateInput">
</template>

<script>
export default {
	name: 'simple-input-box',
	props: {
		modelValue: {
			type: [String, Number],
			required: true
		}
	},
	methods: {
		updateInput(event) {
			this.$emit('update:modelValue', event.target.value)
		}
	}
};
</script>

<style scoped>

</style>