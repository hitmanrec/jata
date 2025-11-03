<template>
	
	<add-item-dialog v-model:show="AddItemDialogVisible">
		<add-item-form 
			@add-item="addItem"
		/>
	</add-item-dialog>
	<div>
		<small-button class="add-item-button" @click="AddItemDialogVisible = true">
			+ Add
		</small-button>
		<transition-group name="item-list">
			
			<simple-item class="list-item"
				v-for="item in items" 
				:item="item"
				:key="item.id"
				@delete-item="$emit('delete-item', item)"
			/>		
		</transition-group>
	</div>
</template>

<script>
import SimpleItem from '@/components/SimpleItem'
import AddItemDialog from '@/components/UI/AddItemDialog'
import AddItemForm from '@/components/AddItemForm'

export default {
	components: {
		SimpleItem,
		AddItemDialog,
		AddItemForm
	},
	props: {
		items: {
			type: Array,
			required: true
		},
		category: {
			type: Object,
			default: () => ({name: '', id: 0, removed: false})
		}
	},
	data() {
		return {
			AddItemDialogVisible: false
		}
	},
	methods: {
		addItem(item) {
			this.$emit('add-item', item)
			this.AddItemDialogVisible = false
		},
	}
}
</script>

<style scoped>
.list-item{
	margin-top: 10px;
}

.add-item-button{
	margin: 10px auto;
	
}

.item-list-item{
	display: inline-block;
	margin-right: 10px;
}

.item-list-enter-active,
.item-list-leave-active{
	transition: all 0.5s ease;
}

.item-list-enter-from,
.item-list-leave-to{
	opacity: 0;
	transform: translateX(100px);

}

.item-list-move{
	transition: transform 0.5s ease;
}
</style>