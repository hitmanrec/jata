<template>
	<div v-if="items.length > 0">
		<h4>Films List</h4>
		<simple-item class="list-item"
			v-for="item in items" 
			:item="item"
			@delete-item="$emit('delete-item', item)"
		/>
	</div>
	<div v-else>
		<h4>Add something</h4>
	</div>
</template>

<script>
import SimpleItem from '@/components/SimpleItem'

export default {
	components: {
		SimpleItem
	},
	props: {
		items: {
			type: Array,
			required: true
		}
	}
}
</script>

<style scoped>
.list-item{
	margin-top: 10px;
}
</style>