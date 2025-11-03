<template>
	<select class="base-select" :value="modelValue" @change="select_changed">
		<option disabled value="">{{ placeholder }}</option>
		<option 
			v-for="option in select_options" 
			:key="option.value" 
			:value="option.value"
		>
			{{ option.label }}
		</option>
	</select>
</template>

<script>
export default {
	name: 'drop-down-dynamic-select',
	props: {
		modelValue: {
			type: String
		},
		placeholder: {
			type: String,
			default: 'No option selected'
		},
		select_options: {
			type: Array,
			default: () => []
		}
	},
	methods: {
		select_changed(event) {
			this.$emit('update:modelValue', event.target.value)
		}
	}

};
</script>

<style scoped>

</style>